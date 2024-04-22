#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <string>
#include <chrono>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <ostream>
#include <set>

using std::string;
using std::vector;

class GameLauncher
{
    class GameBoard
    {
        class GameButton
        {
            protected:
                sf::Sprite buttonSprite;
                std::map<string, sf::Sprite> spriteMap = { {"box", buttonSprite} };

            public:
                GameButton() {};
                GameButton(sf::Texture& textureData, const float& xPos, const float& yPos);
                void drawButtonSprite(sf::RenderWindow& gameWindow, bool isGamePaused = false);
                bool isInsideSprite(const sf::Vector2f& coordinates);
                void setButtonSprite(const string& spriteName, sf::Texture& textureData);
                void addButtonSprite(const sf::Texture& textureData, const string& spriteId, const float& xPos = 0, const float& yPos = 0);
                void deleteButtonSprite(const string& spriteName);
        };

        class GameTile : public GameButton
        {
            bool isTileMine = false, hasTileFlag = false, isTileRevealed = false;
            int tileDanger = 0;
            vector<GameTile*> tilePointerList;
            vector<GameTile*> safeTileList;

            public:
                GameTile(sf::Texture& textureData, const int& xPos, const int& yPos);
                void setTilePointer(GameTile* adjacentTile);
                bool setTileMine();
                void setTileSafe(sf::Texture& textureData);
                void revealTile(const std::map<string, sf::Texture>& textureMap, int& revealedCount, std::set<GameTile*> adjacentTiles = {}); 
                bool isTileSeen();
                bool isTileBomb();
                void updateSafeTile();
                bool toggleTileFlag(sf::Texture& textureData, bool gameWon = false);
                sf::Vector2f getTilePosition();
        };
        
        string playerNickName = "";
        string* lastGameWinner = nullptr;
        int boardWidth = 25, boardHeight = 16, totalMines = 50, safeTileCount = 0;
        int flagCounter = 0, revealedTiles = 0, gameTimer = 0;
        bool displayMines = false, gameEnded = false, gamePaused = false, countNegative = true, debugMode = false;

        sf::RenderWindow* leaderboardWindow;
        sf::Text* winnerText;
        vector<string> leaderboardNames;

        vector<vector<GameTile>> gameTileBoard;
        vector<GameTile*> mineTileList;
        vector<sf::Sprite> mineSpriteField;
        std::map<string, GameButton> gameButtons;
        std::time_t gameStartTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        sf::Texture textureOne, textureTwo, textureThree, textureFour, textureFive, textureSix, textureSeven, textureEight; 
        sf::Texture textureFlag, textureMine, textureHidden, textureRevealed, textureHappy, textureLose, textureWin, textureLeaderboard, texturePause, texturePlay, textureDebug, textureDigits;
        sf::Texture textureDigit0, textureDigit1, textureDigit2, textureDigit3, textureDigit4, textureDigit5, textureDigit6, textureDigit7, textureDigit8, textureDigit9, textureDigitNegative; 

        std::map<string, sf::Texture> textureList = {
            {"flag", textureFlag},{"mine", textureMine},{"tile_hidden", textureHidden},{"tile_revealed", textureRevealed},
            {"face_happy", textureHappy}, {"face_lose", textureLose},{"face_win", textureWin}, {"leaderboard", textureLeaderboard}, 
            {"pause", texturePause}, {"play", texturePlay}, {"debug", textureDebug}
        };

        void constructGameBoard();
        void placeMines();
        void resetGameBoard();
        void setGameTextures(std::map<int, sf::Texture>& textureMap);
        void setGameTextures(std::map<string, sf::Texture>& textureMap);
        void setGameTextures(vector <std::pair<string, sf::Texture>>& textureVector);
        void createGameButtons(const vector<std::pair<string, std::pair<int, int>>>& buttonVector);
        void updateCounter();
        void readLeaderboard();
        void writeLeaderboard();
        void addLeaderboard();
        void updateLeaderboard();

        public:
            GameBoard(const string& playerName, const int& boardWidth, const int& boardHeight, const int& totalMines, sf::RenderWindow& leaderboardWindow, sf::Text& bestTimesText);
            void drawGameTiles(sf::RenderWindow& gameWindow);
            void buttonClick(const sf::Vector2f& coordinates);
            void flagClick(const sf::Vector2f& coordinates);
            void updateGameTime();
            void updateGameTime(const std::time_t& currentTime);
            void toggleHide(bool hideTiles = false);
    };

    void GameField(const std::string& userName, const int& boardWidth, const int& boardHeight, const int& totalMines);
    sf::Text createCenteredText(const std::string& text, const int& fontSize, const float& xPos, const float& yPos, const sf::Font& font, bool isUnderlined = false);
    void centerTextPosition(sf::Text& words, const float& xPos, const float& yPos);

    public:
        GameLauncher(); 
};