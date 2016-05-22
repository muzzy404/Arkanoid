#include <SFML/Graphics.hpp>
#include <iostream>

#define screenWidth 800
#define screenHight 600
#define ballSpeed 5.0
#define ballRadius 10.0

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

class Ball{
	sf::CircleShape shape;
	sf::Vector2f speed{-ballSpeed, -ballSpeed};
public:
	Ball(float startX, float startY)
    {
        shape.setPosition(startX, startY);
        shape.setRadius(ballRadius);
        shape.setFillColor(sf::Color::Blue);
        shape.setOrigin(ballRadius, ballRadius);
	};
	sf::CircleShape getBall(){ return shape; }
	//получить параметры мячика
	float x()      { return shape.getPosition().x; }
	float y()      { return shape.getPosition().y; }
	float left()   { return x() - shape.getRadius(); }
	float right()  { return x() + shape.getRadius(); }
	float top()    { return y() - shape.getRadius(); }
	float bottom() { return y() + shape.getRadius(); }
	void update();
};

void Ball::update()
{
	shape.move(speed);
	//Мячик должен находиться внутри экрана
	//Проверки местоположения и последующие настойки скорости
    //горизонтальная скорость
	if(left() < 0) speed.x = ballSpeed;
		else if(right() > screenWidth) speed.x = -ballSpeed;
    //вертикальная скорость
    if(top() < 0) speed.y = ballSpeed;
		else if(bottom() > screenHight) speed.y = -ballSpeed;
    }

int main()
{
	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHight), "Arkanoid");
	window.setFramerateLimit(60);
	
	//объекты
	Ball ball{screenWidth / 2, screenHight / 2};
	
	while(true)
    {
        window.clear(sf::Color::Black);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) break;

        ball.update();

        window.draw(ball.getBall());
        window.display();
    }
	return 0;
}