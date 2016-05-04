#include <SFML/Graphics.hpp>
#include <iostream>

#define screenWidth 800
#define screenHight 600

int main()
{
	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHight), "Arkanoid");

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape)
					window.close();
			}
		}

		window.clear();
		window.display();
	}

	return 0;
}