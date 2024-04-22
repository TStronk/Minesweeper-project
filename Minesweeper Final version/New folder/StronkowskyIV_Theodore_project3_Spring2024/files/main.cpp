#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "GameCell.h"
#include "GameMaster.h"
using namespace sf;
using namespace std;

int main()
{
    sf::RenderWindow screen(sf::VideoMode(800, 576), "Minesweeper");

    GameMaster gameMaster;

    while (screen.isOpen())
    {
        sf::Event event;
        while (screen.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                screen.close();

            screen.clear(Color(255, 255, 255, 255));

            gameMaster.update(screen);

            screen.display();
        }
    }

    return 0;
}
