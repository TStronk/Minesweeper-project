#include "GameBoard.h"

GameLauncher::GameBoard::GameBoard(const string& name, const int& wide, const int& tall, const int& total, sf::RenderWindow& leaderboardWindow, sf::Text& bestTimes)
{
	this->boardWidth = wide;
	this->boardHeight = tall;
	this->totalMines = total;
	this->safeTileCount = wide * tall - total;
	this->playerNickName = name;
	this->flagCounter = 0;
	this->leaderboardWindow = &leaderboardWindow; 
	this->winnerText = &bestTimes; 
	srand((unsigned)time(NULL) + boardWidth + boardHeight + totalMines + playerNickName.length());

	setGameTextures(textureList);
	std::map<int, sf::Texture> numList = { {1, textureOne},{2, textureTwo},{3, textureThree},{4, textureFour},{5, textureFive},{6, textureSix},{7, textureSeven},{8, textureEight} };  
	vector <std::pair<string, sf::Texture>> tmpList = { {"digit0", textureDigit0},{"digit1", textureDigit1},{"digit2", textureDigit2},{"digit3", textureDigit3},{"digit4", textureDigit4},{"digit5", textureDigit5},{"digit6", textureDigit6},{"digit7", textureDigit7},{"digit8", textureDigit8},{"digit9", textureDigit9},{"digitn", textureDigitNegative} };
	setGameTextures(numList);
	setGameTextures(tmpList);
	constructGameBoard();
	createGameButtons({
		{"face_happy",{(boardWidth / 2.0) * 32 - 32,32 * (boardHeight + 0.5f)}},
		{"debug",{boardWidth * 32 - 304,32 * (boardHeight + 0.5f)}},
		{"pause",{boardWidth * 32 - 240,32 * (boardHeight + 0.5f)}},
		{"leaderboard",{boardWidth * 32 - 176,32 * (boardHeight + 0.5f)}}
		});

	readLeaderboard();
	updateLeaderboard();
	updateCounter();
}

void GameLauncher::GameBoard::constructGameBoard()
{
	
	gameTileBoard.clear();

	
	for (int i = 0; i < boardHeight; i++)
	{
		vector<GameTile> newLine;

		for (int j = 0; j < boardWidth; j++)
		{
			GameTile newTile(textureList.at("tile_hidden"), j, i);
			newLine.push_back(newTile);
		}

		gameTileBoard.push_back(newLine);
	}

	
	for (int i = 0; i < boardHeight; i++)
	{
		for (int j = 0; j < boardWidth; j++)
		{
			if (j + 1 != boardWidth) gameTileBoard.at(i).at(j).setTilePointer(&gameTileBoard.at(i).at(j + 1)); 
			if (i > 0) { 
				gameTileBoard.at(i).at(j).setTilePointer(&gameTileBoard.at(i - 1).at(j)); 
				if (j + 1 != boardWidth) gameTileBoard.at(i).at(j).setTilePointer(&gameTileBoard.at(i - 1).at(j + 1));	
				if (j > 0) gameTileBoard.at(i).at(j).setTilePointer(&gameTileBoard.at(i - 1).at(j - 1)); 
			}
		}
	}

	placeMines();
}

void GameLauncher::GameBoard::placeMines()
{
	int count = 0, randRow = 0, randCol = 0;
	while (count < totalMines)
	{
		
		randRow = rand() % gameTileBoard.size();
		randCol = rand() % gameTileBoard[0].size();

		
		if (gameTileBoard.at(randRow).at(randCol).setTileMine()) {
			count += 1;

			sf::Sprite mine;
			mineSpriteField.push_back(mine);
			mineSpriteField.back().setTexture(textureList.at("mine"));
			mineSpriteField.back().setPosition(gameTileBoard.at(randRow).at(randCol).getTilePosition());
			mineTileList.push_back(&gameTileBoard.at(randRow).at(randCol));
		}
	}

	for (auto& line : gameTileBoard)
	{
		for (auto& tile : line)
		{
			tile.updateSafeTile();
		}
	}
}

void GameLauncher::GameBoard::resetGameBoard()
{
	mineSpriteField.clear();
	mineTileList.clear();
	if (!debugMode) displayMines = false;
	gameEnded = false;
	gamePaused = false;
	flagCounter = 0;
	revealedTiles = 0;
	gameTimer = 0;
	updateGameTime(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
	gameButtons.at("clock").setButtonSprite("timML", textureList.at("digit0"));
	gameButtons.at("clock").setButtonSprite("timMR", textureList.at("digit0"));
	gameButtons.at("clock").setButtonSprite("timSL", textureList.at("digit0"));
	gameButtons.at("clock").setButtonSprite("timSR", textureList.at("digit0"));
	gameButtons.at("face_happy").setButtonSprite("box", textureList.at("face_happy"));

	for (auto& line : gameTileBoard)
	{
		for (auto& tile : line)
		{
			tile.setTileSafe(textureList.at("tile_hidden"));
		}
	}

	updateCounter();
	placeMines();
}

void GameLauncher::GameBoard::setGameTextures(std::map<int, sf::Texture>& m)
{
	string filename = "";
	std::map<int, sf::Texture>::iterator iter;
	for (iter = m.begin(); iter != m.end(); iter++)
	{
		filename = "files/images/number_" + std::to_string(iter->first) + ".png";
		iter->second.loadFromFile(filename);
		textureList.insert({ std::to_string(iter->first), iter->second });
	}
}

void GameLauncher::GameBoard::setGameTextures(std::map<string, sf::Texture>& m)
{
	string filename = "";
	std::map<string, sf::Texture>::iterator iter;
	for (iter = m.begin(); iter != m.end(); iter++)
	{
		filename = "files/images/" + iter->first + ".png";
		iter->second.loadFromFile(filename);
	}
}

void GameLauncher::GameBoard::setGameTextures(vector <std::pair<string, sf::Texture>>& v)
{
	for (int i = 0; i < v.size(); i++)
	{
		v.at(i).second.loadFromFile("files/images/digits.png", sf::IntRect(21 * i, 0, 21, 32));
		textureList.insert({ v.at(i).first, v.at(i).second });
	}
}

void GameLauncher::GameBoard::createGameButtons(const vector<std::pair<string, std::pair<int, int>>>& v)
{
	for (auto& para : v)
	{
		GameButton but(textureList.at(para.first), para.second.first, para.second.second);
		gameButtons.emplace(para.first, but);
	}

	GameButton cnt(textureList.at("digitn"), 12, 32 * (boardHeight + 0.5f) + 16);
	gameButtons.emplace("count", cnt);
	gameButtons.at("count").addButtonSprite(textureList.at("digit0"), "cntH", 33, 32 * (boardHeight + 0.5f) + 16);
	gameButtons.at("count").addButtonSprite(textureList.at("digit0"), "cntT", 54, 32 * (boardHeight + 0.5f) + 16);
	gameButtons.at("count").addButtonSprite(textureList.at("digit0"), "cntO", 75, 32 * (boardHeight + 0.5f) + 16);

	GameButton clk(textureList.at("digit0"), 0, 0);
	gameButtons.emplace("clock", clk);
	gameButtons.at("clock").deleteButtonSprite("box");
	gameButtons.at("clock").addButtonSprite(textureList.at("digit0"), "timML", boardWidth * 32 - 97, 32 * (boardHeight + 0.5f) + 16);
	gameButtons.at("clock").addButtonSprite(textureList.at("digit0"), "timMR", boardWidth * 32 - 76, 32 * (boardHeight + 0.5f) + 16);
	gameButtons.at("clock").addButtonSprite(textureList.at("digit0"), "timSL", boardWidth * 32 - 54, 32 * (boardHeight + 0.5f) + 16);
	gameButtons.at("clock").addButtonSprite(textureList.at("digit0"), "timSR", boardWidth * 32 - 33, 32 * (boardHeight + 0.5f) + 16);
}

void GameLauncher::GameBoard::toggleHide(bool hide)
{
	for (auto& line : gameTileBoard)
	{
		for (auto& tile : line)
		{
			if (gamePaused || hide) tile.setButtonSprite("box", textureList.at("tile_revealed"));
			else if (!tile.isTileSeen()) tile.setButtonSprite("box", textureList.at("tile_hidden"));
		}
	}
}

void GameLauncher::GameBoard::updateCounter()
{
	int count = totalMines - flagCounter;
	if (count >= 0 && countNegative) { gameButtons.at("count").deleteButtonSprite("box"); countNegative = false; }
	else if (count < 0 && !countNegative) { gameButtons.at("count").addButtonSprite(textureList.at("digitn"), "box", 12, 32 * (boardHeight + 0.5f) + 16); countNegative = true; }

	count = std::abs(count);
	gameButtons.at("count").setButtonSprite("cntH", textureList.at("digit" + std::to_string(count / 100 % 10)));
	gameButtons.at("count").setButtonSprite("cntT", textureList.at("digit" + std::to_string(count / 10 % 10)));
	gameButtons.at("count").setButtonSprite("cntO", textureList.at("digit" + std::to_string(count % 10)));
}

void GameLauncher::GameBoard::readLeaderboard()
{
	std::ifstream file("files/leaderboard.txt");
	if (file.is_open())
	{
		string line;
		while (std::getline(file, line)) {
			leaderboardNames.push_back(line);
		}
	}
	std::sort(leaderboardNames.begin(), leaderboardNames.end());
	file.close();
}

void GameLauncher::GameBoard::writeLeaderboard()
{
	std::ofstream file;
	file.open("files/leaderboard.txt", std::ofstream::out | std::ofstream::trunc);
	if (file.is_open())
		for (auto& name : leaderboardNames) {
			file << name;
			if (name != leaderboardNames.at(leaderboardNames.size() - 1))
				file << "\n";
		}

	file.close();
	*lastGameWinner += "*";
}

void GameLauncher::GameBoard::addLeaderboard()
{
	int minutes = gameTimer / 60;
	int seconds = gameTimer - (minutes * 60);
	string mL = std::to_string(minutes / 10 % 10);
	string mR = std::to_string(minutes % 10);
	string sL = std::to_string(seconds / 10 % 10);
	string sR = std::to_string(seconds % 10);
	string newTime = mL + mR + ":" + sL + sR + ',' + playerNickName;

	if (leaderboardNames.size() == 0) { 
		leaderboardNames.push_back(newTime);
		lastGameWinner = &leaderboardNames.at(0);
		writeLeaderboard();
	}
	else {
		for (int i = 0; i < leaderboardNames.size(); i++) {
			if (newTime < leaderboardNames.at(i)) {
				if (lastGameWinner != nullptr) lastGameWinner->pop_back(); 	
				leaderboardNames.insert(leaderboardNames.begin() + i, newTime);
				if (leaderboardNames.size() > 5)leaderboardNames.pop_back(); 
				lastGameWinner = &leaderboardNames.at(i);
				writeLeaderboard();
				return;
			}
		}

		if (leaderboardNames.size() < 5) {
			if (lastGameWinner != nullptr) lastGameWinner->pop_back();
			leaderboardNames.push_back(newTime);
			lastGameWinner = &leaderboardNames.at(leaderboardNames.size() - 1);
			writeLeaderboard();
		}
	}
}

void GameLauncher::GameBoard::updateLeaderboard()
{
	string output = "";
	for (int i = 0; i < leaderboardNames.size(); i++)
	{
		output += std::to_string(i + 1) + ".\t" + leaderboardNames.at(i).substr(0, 5) + "\t" + leaderboardNames.at(i).substr(6);
		if (i != leaderboardNames.size()) output += "\n\n";
	}

	winnerText->setString(output);
	sf::FloatRect box = winnerText->getLocalBounds();
	winnerText->setOrigin(box.left + box.width / 2.0f, box.top + box.height / 2.0f);
	winnerText->setPosition(16 * boardWidth / 2.0f, 16 * boardHeight / 2.0f + 20);
}

void GameLauncher::GameBoard::drawGameTiles(sf::RenderWindow& window)
{
	bool hideTiles = false;
	if (gamePaused || (leaderboardWindow->isOpen() && !gameEnded)) hideTiles = true;
	for (auto& line : gameTileBoard)
	{
		for (auto& tile : line)
		{
			tile.drawButtonSprite(window, hideTiles);
		}
	}

	std::map<string, GameButton>::iterator iter;
	for (iter = gameButtons.begin(); iter != gameButtons.end(); iter++) { iter->second.drawButtonSprite(window); }

	if (displayMines && !hideTiles) for (auto& mine : mineSpriteField) { window.draw(mine); }
}

void GameLauncher::GameBoard::buttonClick(const sf::Vector2f& cords)
{
	std::map<string, GameButton>::iterator iter;
	for (iter = gameButtons.begin(); iter != gameButtons.end(); iter++)
	{
		if (iter->second.isInsideSprite(cords))
		{
			if (iter->first == "face_happy") resetGameBoard();
			if (iter->first == "pause" && !gameEnded) {
				gamePaused = !gamePaused; 
				if (gamePaused) {
					iter->second.setButtonSprite("box", textureList.at("play"));
					toggleHide();
				}
				else
				{
					iter->second.setButtonSprite("box", textureList.at("pause"));
					toggleHide();
					updateGameTime(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
				}
			}
			if (iter->first == "debug" && !gamePaused && !gameEnded) { debugMode = !debugMode; displayMines = !displayMines; } 
				
				
			if (iter->first == "leaderboard") {
				leaderboardWindow->create(sf::VideoMode(16 * boardWidth, 16 * boardHeight + 50), "Minesweeper");
				if (!gameEnded) toggleHide(true);
			}
		}
	}

	if (!gameEnded && !gamePaused)
	{
		
		for (auto& line : gameTileBoard)
		{
			for (auto& tile : line)
			{
				if (!tile.isTileSeen() && tile.isInsideSprite(cords)) { 
					if (tile.isTileBomb()) { 
						gameEnded = true;
						displayMines = true;
						for (auto mine : mineTileList) 
							mine->setButtonSprite("box", textureList.at("tile_revealed"));
						gameButtons.at("face_happy").setButtonSprite("box", textureList.at("face_lose"));
					}
					else
					{
						std::set<GameTile*> adjTiles;
						tile.revealTile(textureList, revealedTiles, adjTiles);
						if (revealedTiles == safeTileCount)
						{
							gameEnded = true;
							gameButtons.at("face_happy").setButtonSprite("box", textureList.at("face_win"));
							flagCounter = totalMines;
							updateCounter();
							addLeaderboard();
							updateLeaderboard();
							for (auto& mine : mineTileList) mine->toggleTileFlag(textureList.at("flag"), true);
							leaderboardWindow->create(sf::VideoMode(16 * boardWidth, 16 * boardHeight + 50), "Minesweeper");
						}
					}
				}
			}
		}
	}
}

void GameLauncher::GameBoard::flagClick(const sf::Vector2f& cords)
{
	if (!gamePaused && !gameEnded)
	{
		for (auto& line : gameTileBoard)
		{
			for (auto& tile : line)
			{
				if (tile.isInsideSprite(cords)) 
					if (!tile.isTileSeen()) { 
						if (tile.toggleTileFlag(textureList.at("flag"))) flagCounter += 1; 
						else flagCounter -= 1;
						updateCounter();
					}
			}
		}
	}
}

void GameLauncher::GameBoard::updateGameTime(const std::time_t& time) { gameStartTime = time; } 
void GameLauncher::GameBoard::updateGameTime()
{
	if (!leaderboardWindow->isOpen() && !gamePaused && !gameEnded) {
		std::time_t timeNow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		double diff = difftime(timeNow, gameStartTime);
		if (diff >= 1) {
			gameStartTime = timeNow;
			gameTimer += 1;
			if (gameTimer >= 6000) gameTimer = 0; 

			int minutes = gameTimer / 60;
			int seconds = gameTimer - (minutes * 60);
			gameButtons.at("clock").setButtonSprite("timML", textureList.at("digit" + std::to_string(minutes / 10 % 10)));
			gameButtons.at("clock").setButtonSprite("timMR", textureList.at("digit" + std::to_string(minutes % 10)));
			gameButtons.at("clock").setButtonSprite("timSL", textureList.at("digit" + std::to_string(seconds / 10 % 10)));
			gameButtons.at("clock").setButtonSprite("timSR", textureList.at("digit" + std::to_string(seconds % 10)));
		}
	}
}