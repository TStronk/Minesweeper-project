#include <SFML/Graphics.hpp>
#include <string>
using namespace sf;
using namespace std;


class GameCell
{
    
    Sprite baseLayer;
    Sprite topLayer;
    Sprite symbolLayer;
    Sprite markerLayer;

    bool debugModeActive;

public:
    
    vector<GameCell*> adjacentCells;
    int mines;
    bool containsMine;
    bool isMarked;
    bool isVisible;

    
    GameCell(map<string,Texture> &resourceTextures, float xCoordinate, float yCoordinate);
    void setMinePresence(bool hasMine);
    void refreshSymbol(map<string, Texture> &resourceTextures);

    
    bool switchMarker();
    void switchDebugMode();
    bool uncoverCell(int &uncoveredCount);
    void renderCell(RenderWindow &window);
};


GameCell::GameCell(map<string,Texture> &resourceTextures, float xCoordinate, float yCoordinate)
{
    baseLayer.setPosition(xCoordinate, yCoordinate);
    topLayer.setPosition(xCoordinate, yCoordinate);
    markerLayer.setPosition(xCoordinate, yCoordinate);
    symbolLayer.setPosition(xCoordinate, yCoordinate);

    
    baseLayer.setTexture(resourceTextures["revealed"]);
    topLayer.setTexture(resourceTextures["hidden"]);
    markerLayer.setTexture(resourceTextures["flag"]);

   
    markerLayer.setColor(Color(255, 255, 255, 0));
    containsMine = false;
    isVisible = false;
    isMarked = false;
    debugModeActive = false;
}


void GameCell::setMinePresence(bool hasMine)
{
    containsMine = hasMine;
}


void GameCell::refreshSymbol(map<string, Texture> &resourceTextures)
{
    mines = 0;
    for (int i = 0; i < 8; i++)
    {
        if (adjacentCells[i] != nullptr && adjacentCells[i]->containsMine)
        {
            mines++;
        }
    }
    if (containsMine)
        symbolLayer.setTexture(resourceTextures["bomb"]);
    else
    {
        
        switch (mines)
        {
            case 1: symbolLayer.setTexture(resourceTextures["one"]); break;
            case 2: symbolLayer.setTexture(resourceTextures["two"]); break;
            case 3: symbolLayer.setTexture(resourceTextures["three"]); break;
            case 4: symbolLayer.setTexture(resourceTextures["four"]); break;
            case 5: symbolLayer.setTexture(resourceTextures["five"]); break;
            case 6: symbolLayer.setTexture(resourceTextures["six"]); break;
            case 7: symbolLayer.setTexture(resourceTextures["seven"]); break;
            case 8: symbolLayer.setTexture(resourceTextures["eight"]); break;

            default: symbolLayer.setTexture(resourceTextures["revealed"]); break;
        }
    }
    symbolLayer.setColor(Color(255, 255, 255, 0));
    markerLayer.setColor(Color(255, 255, 255, 0));
    topLayer.setColor(Color(255, 255, 255, 255));
    isVisible = false;
    isMarked = false;
    debugModeActive = false;
}


bool GameCell::switchMarker()
{
    if (!isVisible)
    {
        isMarked = !isMarked;
        markerLayer.setColor(isMarked ? Color(255, 255, 255, 255) : Color(255, 255, 255, 0));
    }
    return isMarked;
}


void GameCell::switchDebugMode()
{
    if (containsMine && !isVisible)
    {
        debugModeActive = !debugModeActive;
        symbolLayer.setColor(debugModeActive ? Color(255, 255, 255, 255) : Color(255, 255, 255, 0));
    }
}


bool GameCell::uncoverCell(int &uncoveredCount)
{
    if (!isVisible && !isMarked)
    {
        isVisible = true;
        uncoveredCount++;
        topLayer.setColor(Color(255, 255, 255, 0));
        symbolLayer.setColor(Color(255, 255, 255, 255));
        if (mines == 0 && !containsMine)
        {
            for (int i = 0; i < 8; i++)
            {
                if (adjacentCells[i] != nullptr && !(adjacentCells[i]->containsMine))
                    adjacentCells[i]->uncoverCell(uncoveredCount);
            }
        }
    }
    return containsMine;
}


void GameCell::renderCell(RenderWindow &window)
{
    window.draw(baseLayer);
    window.draw(topLayer);
    window.draw(symbolLayer);
    window.draw(markerLayer);
}

