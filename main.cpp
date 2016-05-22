#include <SFML/Graphics.hpp>
#include <iostream>

#define screenWidth 1000
#define screenHight 600
#define ballRadius 8.0
#define gameFieldWidth (0.60 * screenWidth)
#define gameFieldHight (0.80 * screenHight)
#define fieldBorderX   (0.05 * screenWidth)
#define fieldBorderY   (0.10 * screenHight)


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
	sf::Vector2f speed;
	float d_speed;
public:
	Ball(float startX, float startY, float ballSpeed)
    {
		d_speed = ballSpeed;
		speed.x = d_speed;
		speed.y = d_speed;
		shape.setPosition(startX, startY);
        shape.setRadius(ballRadius);
        shape.setFillColor(sf::Color::Blue);
        shape.setOrigin(ballRadius, ballRadius);
	};
	void set_d_speed(float newSpeed){ d_speed = newSpeed; }
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

class BackGround{
	sf::RectangleShape shape;
public:
	BackGround(float startX, float startY)
	{
		shape.setPosition(startX, startY);
		sf::Vector2f gameFieldSize{gameFieldWidth, gameFieldHight};
		shape.setSize(gameFieldSize);
		shape.setFillColor(sf::Color::Black);
	};
	sf::RectangleShape getBackGround(){ return shape; }
};

void Ball::update()
{
	shape.move(speed);
	//Мячик должен находиться внутри экрана
	//Проверки местоположения и последующие настойки скорости
    //горизонтальная скорость
	if (left() < fieldBorderX) speed.x = d_speed;
		else if (right() > (fieldBorderX + gameFieldWidth)) speed.x = -d_speed;
    //вертикальная скорость
	if (top() < fieldBorderY) speed.y = d_speed;
	else if (bottom() > (fieldBorderY + gameFieldHight)) speed.y = -d_speed;
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHight), "Arkanoid");
	window.setFramerateLimit(60);
	
	//объекты
	Ball ball{(screenWidth / 2), (screenHight / 2), 6.0};
	BackGround bkGround{fieldBorderX, fieldBorderY};
	
	while(true)
    {
        window.clear(sf::Color::Cyan);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) break;

        ball.update();

		window.draw(bkGround.getBackGround());
        window.draw(ball.getBall());
        window.display();
    }
	return 0;
}