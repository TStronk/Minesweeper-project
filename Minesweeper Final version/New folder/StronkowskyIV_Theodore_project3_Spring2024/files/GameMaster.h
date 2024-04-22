#pragma once
#include "GameCell.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <map>
#include <iostream>
#include <fstream>
using namespace std;
using namespace sf;

class GameMaster
{
    int flagsAvailable;
    int mines;
    int tilesRevealed;

    map<string, Texture> iconTextures;
    vector<bool> cellIds;
    vector<int> testCase1;
    vector<int> testCase2;

    Sprite debugIcon;
    Sprite test1;
    Sprite test2;
    Sprite smiley;

    Sprite hundreds;
    Sprite tens;
    Sprite ones;

    bool isLost;
    bool isOver;

public:
    vector<GameCell> cells;

    GameMaster();

    void randomCellIds();
    void setCellIds(vector<int> &bits, bool oneOrTwo);
    void makeCells();

    void setBombStates();
    void setIcons();

    void update(RenderWindow &window);
    void debug();
    void draw(RenderWindow &window);

    void refreshCounter();
    void refreshSmiley();
};


GameMaster::GameMaster()
{
    srand(time(NULL));
    flagsAvailable = 50;

    fstream file;
    string lineHolder;
    

    Texture holder;
    holder.loadFromFile("images/debug.png"); 
    iconTextures.emplace("debug", holder);

    holder.loadFromFile("images/digits.png"); 
    iconTextures.emplace("digits", holder);

    holder.loadFromFile("images/face_happy.png"); 
    iconTextures.emplace("happy", holder);
    holder.loadFromFile("images/face_lose.png");
    iconTextures.emplace("lose", holder);
    holder.loadFromFile("images/face_win.png");
    iconTextures.emplace("win", holder);

    holder.loadFromFile("images/flag.png");
    iconTextures.emplace("flag", holder);
    holder.loadFromFile("images/mine.png");
    iconTextures.emplace("bomb", holder);
    holder.loadFromFile("images/number_1.png");
    iconTextures.emplace("one", holder);
    holder.loadFromFile("images/number_2.png");
    iconTextures.emplace("two", holder);
    holder.loadFromFile("images/number_3.png");
    iconTextures.emplace("three", holder);
    holder.loadFromFile("images/number_4.png");
    iconTextures.emplace("four", holder);
    holder.loadFromFile("images/number_5.png");
    iconTextures.emplace("five", holder);
    holder.loadFromFile("images/number_6.png");
    iconTextures.emplace("six", holder);
    holder.loadFromFile("images/number_7.png");
    iconTextures.emplace("seven", holder);
    holder.loadFromFile("images/number_8.png");
    iconTextures.emplace("eight", holder);

    holder.loadFromFile("images/cell_hidden.png"); 
    iconTextures.emplace("hidden", holder);
    holder.loadFromFile("images/cell_revealed.png");
    iconTextures.emplace("revealed", holder);

    debugIcon.setTexture(iconTextures["debug"]);
    debugIcon.setPosition(496, 512);
  
    smiley.setTexture(iconTextures["happy"]);
    smiley.setPosition(368, 512);

    hundreds.setTexture(iconTextures["digits"]);
    hundreds.setTextureRect(Rect<int>(0, 0, 21, 32));
    hundreds.setPosition(0, 512);
    tens.setTexture(iconTextures["digits"]);
    tens.setTextureRect(Rect<int>(21 * 5, 0, 21, 32));
    tens.setPosition(21, 512);
    ones.setTexture(iconTextures["digits"]);
    ones.setTextureRect(Rect<int>(0, 0, 21, 32));
    ones.setPosition(42, 512);


    makeCells();
    randomCellIds();
}

void GameMaster::randomCellIds()
{
    cellIds.clear();
    vector<int> bits;
    int randomNum;
    bool getFree = false;

    for (int c = 0; c < 50; c++)
    {
        while (!getFree)
        {
            randomNum = rand() % 400;
            if (find(bits.begin(), bits.end(), randomNum) == bits.end())
            {
                bits.push_back(randomNum);
                getFree = true;
            }
        }
        getFree = false;
    }

    auto iter = bits.begin();
    for (int d = 0; d < 400; d++)
    {
        iter = find(bits.begin(), bits.end(), d);
        if (iter == bits.end())
        {
            cellIds.push_back(false);
        }
        else
        {
            cellIds.push_back(true);
        }
    }
    setBombStates();
    setIcons();
    flagsAvailable = 50;
    refreshCounter();
    mines = 50;
    tilesRevealed = 0;

    isLost = false;
    isOver = false;
}

void GameMaster::setCellIds(vector<int>&bits, bool oneOrTwo)
{
    cellIds.clear();
    for (int c = 0; c < 400; c++)
    {
        if (bits[c] == 0)
        {
            cellIds.push_back(false);
        }
        else
        {
            cellIds.push_back(true);
        }
    }
    setBombStates();
    setIcons();
    refreshCounter();
    if (oneOrTwo)
        mines = 1;
    else mines = 98;

    flagsAvailable = mines;
    refreshCounter();

    isLost = false;
    isOver = false;
}

void GameMaster::makeCells()
{
    float x = 0; float y = 0;
    for (int c = 0; c < 16; c++)
    {
        for (int d = 0; d < 25; d++)
        {
            GameCell cell = GameCell(iconTextures, x, y);
            cells.push_back(cell);
            x += 32.f;
        }
        x = 0;
        y += 32.f;
    }
}

void GameMaster::setBombStates()
{
    for (int c = 0; c < 400; c++)
    {
        cells[c].setMinePresence(cellIds[c]);
    }
}

void GameMaster::setIcons()
{
    for (int c = 0; c < 400; c++)
    {
        cells[c].adjacentCells.clear();
        for (int d = 0; d < 8; d++)
        {
            cells[c].adjacentCells.push_back(nullptr);
        }
        if (c == 0) 
        {
            cells[c].adjacentCells[4] = &(cells[c + 1]);
            cells[c].adjacentCells[6] = &(cells[c + 25]);
            cells[c].adjacentCells[7] = &(cells[c + 26]);
        }
        else if (c == 24) 
        {
            cells[c].adjacentCells[3] = &(cells[c - 1]);
            cells[c].adjacentCells[5] = &(cells[c + 24]);
            cells[c].adjacentCells[6] = &(cells[c + 25]);
        }
        else if (c == 375) 
        {
            cells[c].adjacentCells[4] = &(cells[c + 1]);
            cells[c].adjacentCells[1] = &(cells[c - 25]);
            cells[c].adjacentCells[2] = &(cells[c - 24]);
        }
        else if (c == 399) 
        {
            cells[c].adjacentCells[3] = &(cells[c - 1]);
            cells[c].adjacentCells[1] = &(cells[c - 25]);
            cells[c].adjacentCells[0] = &(cells[c - 26]);
        }
        else if (c < 24) 
        {
            cells[c].adjacentCells[3] = &(cells[c - 1]);
            cells[c].adjacentCells[4] = &(cells[c + 1]);
            cells[c].adjacentCells[5] = &(cells[c + 24]);
            cells[c].adjacentCells[6] = &(cells[c + 25]);
            cells[c].adjacentCells[7] = &(cells[c + 26]);
        }
        else if (c > 375) 
        {
            cells[c].adjacentCells[3] = &(cells[c - 1]);
            cells[c].adjacentCells[4] = &(cells[c + 1]);
            cells[c].adjacentCells[2] = &(cells[c - 24]);
            cells[c].adjacentCells[1] = &(cells[c - 25]);
            cells[c].adjacentCells[0] = &(cells[c - 26]);
        }
        else if ((c % 25) == 0) 
        {
            cells[c].adjacentCells[4] = &(cells[c + 1]);
            cells[c].adjacentCells[6] = &(cells[c + 25]);
            cells[c].adjacentCells[7] = &(cells[c + 26]);
            cells[c].adjacentCells[1] = &(cells[c - 25]);
            cells[c].adjacentCells[2] = &(cells[c - 24]);
        }
        else if (((c - 24) % 25) == 0) 
        {
            cells[c].adjacentCells[3] = &(cells[c - 1]);
            cells[c].adjacentCells[0] = &(cells[c - 26]);
            cells[c].adjacentCells[1] = &(cells[c - 25]);
            cells[c].adjacentCells[5] = &(cells[c + 24]);
            cells[c].adjacentCells[6] = &(cells[c + 25]);
        }
        else 
        {
            cells[c].adjacentCells[3] = &(cells[c - 1]);
            cells[c].adjacentCells[4] = &(cells[c + 1]);
            cells[c].adjacentCells[5] = &(cells[c + 24]);
            cells[c].adjacentCells[6] = &(cells[c + 25]);
            cells[c].adjacentCells[7] = &(cells[c + 26]);
            cells[c].adjacentCells[2] = &(cells[c - 24]);
            cells[c].adjacentCells[1] = &(cells[c - 25]);
            cells[c].adjacentCells[0] = &(cells[c - 26]);
        }
        cells[c].refreshSymbol(iconTextures);
    }
}

void GameMaster::update(RenderWindow &window)
{
    if (Mouse::isButtonPressed(Mouse::Left))
    {

        Vector2i position = Mouse::getPosition(window);
        if (position.y >= 0 && position.y < 512 && !isOver)
        {
            if (position.x >= 0 && position.x <= 800)
            {
                int x = (position.x / 32);
                int y = (position.y / 32);

                int pos = (y * 25) + x;
                int count = 0;
                if (cells[pos].uncoverCell(count))
                {
                    isLost = true;
                    isOver = true;
                    int dummyCount = 0;
                    for (int c = 0; c < 400; c++)
                    {
                        if (cells[c].isMarked && cells[c].containsMine)
                            cells[c].switchMarker();
                        if (cells[c].containsMine)
                            cells[c].uncoverCell(dummyCount);
                    }
                }
                else tilesRevealed += count;
            }
        }
        else if (position.y >= 512 && position.y < 572)
        {
            if (position.x >= 368 && position.x < 432)
            {
                randomCellIds();
            }
            if (position.x >= 496 && position.x < 560 && !isOver)
            {
                debug();
            }
            if (position.x >= 560 && position.x < 624 && !isOver)
            {
                setCellIds(testCase1, true);
            }
            if (position.x >= 624 && position.x < 688 && !isOver)
            {
                setCellIds(testCase2, false);
            }
        }
    }

    if (Mouse::isButtonPressed(Mouse::Right)&& !isOver)
    {
        Vector2i position = Mouse::getPosition(window);
        if (position.x >= 0 && position.x <= 800)
        {
            if (position.y >= 0 && position.y <= 512)
            {
                int x = (position.x / 32);
                int y = (position.y / 32);

                int pos = (y * 25) + x;
                if (!cells[pos].isMarked && flagsAvailable > 0 && !cells[pos].isVisible)
                {
                    cells[pos].switchMarker();
                    flagsAvailable--;
                }
                else if (!cells[pos].isVisible && cells[pos].isMarked)
                {
                    cells[pos].switchMarker();
                    flagsAvailable++;
                }
                refreshCounter();
            }
        }
    }

    if (tilesRevealed == (400 - mines) && !isLost)
    {
        isOver = true;
        for (int e = 0; e < 400; e++)
        {
            if (cells[e].containsMine && !cells[e].isMarked)
            {
                cells[e].switchMarker();
            }
        }
    }

    refreshSmiley();

    draw(window);
}

void GameMaster::debug()
{
    for (int c = 0; c < 400; c++)
    {
        cells[c].switchDebugMode();
    }
}

void GameMaster::draw(RenderWindow &window)
{
    for (int c = 0; c < 400; c++)
        cells[c].renderCell(window);

    window.draw(smiley);
    window.draw(debugIcon);
  
    window.draw(hundreds);
    window.draw(tens);
    window.draw(ones);
}

void GameMaster::refreshCounter()
{
    int copy = flagsAvailable;
    int onesPlace = copy % 10;
    copy /= 10;
    int tensPlace = copy % 10;

    tens.setTextureRect(Rect<int>(21 * tensPlace, 0, 21, 32));
    ones.setTextureRect(Rect<int>(21 * onesPlace, 0, 21, 32));
}

void GameMaster::refreshSmiley()
{
    if (isOver)
    {
        if (isLost)
            smiley.setTexture(iconTextures["lose"]);
        else
            smiley.setTexture(iconTextures["win"]);
    }
    else smiley.setTexture(iconTextures["happy"]);
}
