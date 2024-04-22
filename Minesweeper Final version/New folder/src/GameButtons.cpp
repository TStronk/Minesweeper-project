#include "GameBoard.h"

GameLauncher::GameBoard::GameButton::GameButton(sf::Texture& texture, const float& xPos, const float& yPos)
{
    spriteMap.at("box").setTexture(texture);
    spriteMap.at("box").setPosition(xPos, yPos);
}

void GameLauncher::GameBoard::GameButton::drawButtonSprite(sf::RenderWindow& window, bool isPaused)
{
    if (!isPaused)
    {
        std::map<std::string, sf::Sprite>::iterator iter;
        for (iter = spriteMap.begin(); iter != spriteMap.end(); iter++)
        {
            window.draw(iter->second);
        }
    }
    else
    {
        window.draw(spriteMap.at("box"));
    }
}

bool GameLauncher::GameBoard::GameButton::isInsideSprite(const sf::Vector2f& coordinates) {
    if (spriteMap.find("box") != spriteMap.end())
        return spriteMap.at("box").getGlobalBounds().contains(coordinates);
    else
        return false;
}

void GameLauncher::GameBoard::GameButton::setButtonSprite(const std::string& name, sf::Texture& texture) { spriteMap.at(name).setTexture(texture); }
void GameLauncher::GameBoard::GameButton::deleteButtonSprite(const std::string& name) { spriteMap.erase(name); }
void GameLauncher::GameBoard::GameButton::addButtonSprite(const sf::Texture& texture, const std::string& id, const float& xPos, const float& yPos) {
    sf::Sprite tmp;
    spriteMap.insert({ id, tmp });
    spriteMap.at(id).setTexture(texture);
    if (xPos > 0 || yPos > 0)
        spriteMap.at(id).setPosition(xPos, yPos);
    else
        spriteMap.at(id).setPosition(spriteMap.at("box").getPosition());
}



GameLauncher::GameBoard::GameTile::GameTile(sf::Texture& texture, const int& xPos, const int& yPos)
{
    spriteMap.at("box").setTexture(texture);
    spriteMap.at("box").setPosition(xPos * 32, yPos * 32);
}

void GameLauncher::GameBoard::GameTile::setTilePointer(GameTile* adjacent)
{
    this->tilePointerList.push_back(adjacent);
    adjacent->tilePointerList.push_back(this);
}

bool GameLauncher::GameBoard::GameTile::setTileMine()
{
    if (!isTileMine) {
        isTileMine = true;

        for (auto& tile : tilePointerList)
        {
            tile->tileDanger += 1;
        }

        return true;
    }
    return false;
}

void GameLauncher::GameBoard::GameTile::setTileSafe(sf::Texture& texture)
{
    hasTileFlag = false;
    isTileRevealed = false;
    isTileMine = false;
    tileDanger = 0;
    spriteMap = { {"box", spriteMap.at("box")} };
    spriteMap.at("box").setTexture(texture);
}

void GameLauncher::GameBoard::GameTile::revealTile(const std::map<std::string, sf::Texture>& textureMap, int& revealCount, std::set<GameTile*> adjacentTiles)
{
    if (adjacentTiles.size() == 0)
        adjacentTiles.emplace(this);
    for (auto& adjacent : adjacentTiles) {
        if (adjacent->isTileRevealed || adjacent->hasTileFlag)
            return;
        adjacent->isTileRevealed = true;
        revealCount += 1;
        adjacent->spriteMap.at("box").setTexture(textureMap.at("tile_revealed"));

        if (adjacent->isTileMine)
            return;
        if (adjacent->tileDanger > 0)
            adjacent->addButtonSprite(textureMap.at(std::to_string(adjacent->tileDanger)), "num");
    }

    std::set<GameTile*> newList;
    for (auto& adjacent : adjacentTiles) {
        adjacent->updateSafeTile();
        if (adjacent->tileDanger == 0)
            for (auto& safe : adjacent->safeTileList)
                newList.emplace(safe);
    }
    revealTile(textureMap, revealCount, newList);
}

bool GameLauncher::GameBoard::GameTile::isTileSeen() { return isTileRevealed; }
bool GameLauncher::GameBoard::GameTile::isTileBomb() { return isTileMine && !hasTileFlag; }

bool GameLauncher::GameBoard::GameTile::toggleTileFlag(sf::Texture& texture, bool gameWon)
{
    hasTileFlag = !hasTileFlag;
    if (hasTileFlag || gameWon)
        addButtonSprite(texture, "flag");
    else
        spriteMap.erase("flag");
    return hasTileFlag;
}

void GameLauncher::GameBoard::GameTile::updateSafeTile()
{
    safeTileList.clear();
    for (auto& adjacent : tilePointerList)
        if (!adjacent->isTileMine && !adjacent->isTileRevealed && !adjacent->hasTileFlag)
            safeTileList.push_back(adjacent);
}

sf::Vector2f GameLauncher::GameBoard::GameTile::getTilePosition() { return spriteMap.at("box").getPosition(); }


