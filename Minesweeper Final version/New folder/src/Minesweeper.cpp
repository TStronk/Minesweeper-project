#include "GameBoard.h"

GameLauncher::GameLauncher()
{
	int Width = 40, Height = 30, mineCount = 100;
	std::ifstream file("files/board_config.cfg");
	if (file.is_open())
	{
		try {
			string Number;
			std::getline(file, Number);
			Width = stoi(Number);
			if (Width < 22) Width = 22; 
			if (Width < 22) Width = 22; 
			if (Width > 255) Width = 255;
			std::getline(file, Number);
			Height = stoi(Number);
			if (Height < 16) Height = 16;
			if (Height > 250) Height = 250;
			std::getline(file, Number);
			mineCount = stoi(Number);
			if (mineCount > Width * Height) mineCount = Width * Height;
		}
		catch (std::invalid_argument) 
		{
			std::cout << "Values in Config.cfg are not integers." << std::endl;
		}
	}
	file.close();

	
	sf::RenderWindow welcome(sf::VideoMode(32 * Width, 32 * Height + 100), "Minesweeper");
	welcome.setFramerateLimit(60);
	sf::Font font;
	font.loadFromFile("files/font.ttf");

	sf::Text greet = createCenteredText("WELCOME TO MINESWEEPER!", 24, 32 * Width, 32 * Height / 2.0f - 150, font, true);
	sf::Text ask = createCenteredText("Enter your name:", 20, 32 * Width, 32 * Height / 2.0f - 75, font);
	sf::Text name = createCenteredText("", 18, 32 * Width, 32 * Height / 2.0f - 45, font);
	name.setFillColor(sf::Color::Yellow);
	std::string userName = "";

	while (welcome.isOpen())
	{
		sf::Event event;
		while (welcome.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				welcome.close();

			if (event.type == sf::Event::TextEntered && userName.size() < 10)
			{
				char letter = '0';
				if (event.text.unicode < 128) 
					letter = static_cast<char>(event.text.unicode);

				if (isalpha(letter)) {
					if (userName.length() == 0) userName += toupper(letter);
					else  userName += tolower(letter);
				}
					
			}

			if (event.type == sf::Event::KeyPressed && userName.size() > 0)
			{
				if (event.key.code == sf::Keyboard::Backspace)
					userName.pop_back();

				else if (event.key.code == sf::Keyboard::Enter) {
					welcome.close();
					GameField(userName, Width, Height, mineCount);
				}
			}
		}

		welcome.clear(sf::Color(0, 0, 255));
		welcome.draw(greet);
		welcome.draw(ask);
		name.setString(userName + "|");
		centerTextPosition(name, 32 * Width, 32 * Height / 2.0f - 45);
		welcome.draw(name);
		welcome.display();
	}
}

void GameLauncher::GameField(const std::string& userName, const int& Width, const int& Height, const int& mineCount)
{
	sf::RenderWindow game(sf::VideoMode(32 * Width, 32 * Height + 100), "Minesweeper");
	game.setFramerateLimit(60);
	sf::RenderWindow ldrBrd;
	ldrBrd.setFramerateLimit(60);
	sf::Font font;
	font.loadFromFile("files/font.ttf");
	sf::Text leader = createCenteredText("LEADERBOARD", 20, 16 * Width, 16 * Height / 2.0f - 120, font, true);
	sf::Text bestTimes = createCenteredText("", 18, 16 * Width, 16 * Height / 2.0f + 20, font);
	GameBoard board(userName, Width, Height, mineCount, ldrBrd, bestTimes);

	while (game.isOpen())
	{
		sf::Event event;
		while (game.pollEvent(event) && !ldrBrd.isOpen())
		{
			if (event.type == sf::Event::Closed) game.close();
			if (event.type == sf::Event::MouseButtonPressed) 
				if (event.mouseButton.button == sf::Mouse::Left)
					board.buttonClick(game.mapPixelToCoords(sf::Mouse::getPosition(game))); 
				else if (event.mouseButton.button == sf::Mouse::Right)
					board.flagClick(game.mapPixelToCoords(sf::Mouse::getPosition(game)));
		}

		game.clear(sf::Color(255, 255, 255));
		board.updateGameTime();
		board.drawGameTiles(game);
		game.display();

		if (ldrBrd.isOpen())
		{
			while (ldrBrd.pollEvent(event))
			{
				if (event.type == sf::Event::Closed) {
					board.updateGameTime(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())); 
					board.toggleHide();
					ldrBrd.close();
				}
			}

			ldrBrd.clear(sf::Color(0, 0, 255));
			ldrBrd.draw(leader);
			ldrBrd.draw(bestTimes);
			ldrBrd.display();
		}
	}
}

sf::Text GameLauncher::createCenteredText(const std::string& txt, const int& size, const float& x, const float& y, const sf::Font& font, bool isUL)
{
	sf::Text text;
	text.setFont(font);
	text.setString(txt);
	text.setCharacterSize(size);
	if (isUL) text.setStyle(sf::Text::Bold | sf::Text::Underlined);
	else text.setStyle(sf::Text::Bold);
	centerTextPosition(text, x, y);

	return text;
}

void GameLauncher::centerTextPosition(sf::Text& words, const float& x, const float& y)
{
	words.setOrigin(words.getLocalBounds().width / 2, words.getLocalBounds().height / 2);
	words.setPosition(x / 2.0f, y);
}

