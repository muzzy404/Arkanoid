#include <SFML/Graphics.hpp>
#include <iostream>

#define screenWidth 800
#define screenHight 600

class GameData{
	char *lvlMap;
	int score;
	int life;
public:
	GameData();
	void newLvl();
	void addScore(int add) { score += add; };
	int getScore() { return score; };
	int getLife() { return life; };
	void loseLife() { if (life > 0) life--; };
};

class Visual : public sf::Drawable{
	int x, y;
	int width, height;
public:
	Visual() { x = 0; y = 0; };
	Visual(int x0, int y0) { x = x0; y = y0; };
	int getX() { return x; };
	int getY() { return y; };
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};

class Dinamic : public Visual{
	int speed;                  //the numb of cycle till the next redraw method
	int tillRedraw;
	virtual void moveObj() = 0; //change coordinates of object
public:
	Dinamic(int startSpeed) { speed = startSpeed; };
	void drawCycle() { tillRedraw--; };
	int getTillRedraw() { return tillRedraw; };
};

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