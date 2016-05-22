#include <SFML/Graphics.hpp>
#include <iostream>

#define screenWidth 1000
#define screenHight 600
#define ballRadius  8.0
#define platformWidth (6 * ballRadius)  
#define platformHeight  (2 * ballRadius) 
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

class DinamicVisual{
protected:
	sf::Vector2f speed;
	float d_speed;
public:
	virtual void update()  = 0;
	virtual float x() = 0;
	virtual float y() = 0;
	virtual float left() = 0;
	virtual float right() = 0;
	virtual float top() = 0;
	virtual float bottom() = 0;
};

class Ball : public DinamicVisual{
public:
	sf::CircleShape shape;
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
	//получить параметры мячика
	float x()      { return shape.getPosition().x; }
	float y()      { return shape.getPosition().y; }
	float left()   { return x() - shape.getRadius(); }
	float right()  { return x() + shape.getRadius(); }
	float top()    { return y() - shape.getRadius(); }
	float bottom() { return y() + shape.getRadius(); }
	void update();
};

class Platform : public DinamicVisual{
public:
	sf::RectangleShape shape;
	Platform(float startX, float startY, float platformSpeed)
	{
		d_speed = platformSpeed;
		speed.x = 0.0;
		speed.y = 0.0;
		shape.setPosition(startX, startY - platformHeight);
        shape.setSize({platformWidth, platformHeight});
        shape.setFillColor(sf::Color::Red);
        shape.setOrigin((platformWidth / 2.0), (platformHeight / 2.0));
	}
	//получить параметры платформы
	float x()      { return shape.getPosition().x; }
    float y()      { return shape.getPosition().y; }
    float left()   { return x() - shape.getSize().x / 2.0; }
    float right()  { return x() + shape.getSize().x / 2.0; }
    float top()    { return y() - shape.getSize().y / 2.0; }
    float bottom() { return y() + shape.getSize().y / 2.0; }
	void update();
};

class BackGround{
public:
	sf::RectangleShape shape;
	BackGround(float startX, float startY)
	{
		shape.setPosition(startX, startY);
		sf::Vector2f gameFieldSize{gameFieldWidth, gameFieldHight};
		shape.setSize(gameFieldSize);
		shape.setFillColor(sf::Color::Black);
	};
};

void Ball::update()
{
	shape.move(speed);
	//Мячик должен находиться внутри игрового поля
	//Проверки местоположения и последующие настойки скорости
    //горизонтальная скорость
	if (left() < fieldBorderX) speed.x = d_speed;
		else if (right() > (fieldBorderX + gameFieldWidth)) speed.x = -d_speed;
    //вертикальная скорость
	if (top() < fieldBorderY) speed.y = d_speed;
	else if (bottom() > (fieldBorderY + gameFieldHight)) speed.y = -d_speed;
}

void Platform::update()
{
	shape.move(speed);
	//Платформа должена находиться внутри игрового поля
	//Проверка нажатия стрелок клавиатуры для перемещения платфомы
	//Если есть нажание, необходимо изменить скорость
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && left() > fieldBorderX + d_speed)
		speed.x = -d_speed;
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && right() < (fieldBorderX + gameFieldWidth) - d_speed)
			speed.x = d_speed;
		//если нажатия нет или платфома достигла края игрового поля, остановить движение
        else
            speed.x = 0.0;
    }

int main()
{
	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHight), "Arkanoid");
	window.setFramerateLimit(60);
	
	//объекты
	Ball ball{(screenWidth / 2.0), (screenHight / 2.0), 6.0};
	BackGround bkGround{fieldBorderX, fieldBorderY};
	Platform platform{ (gameFieldWidth / 2.0 + fieldBorderX), (fieldBorderY + gameFieldHight), 5.0};

	while(window.isOpen())
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
		
		ball.update();
		platform.update();
		
		window.clear(sf::Color::White);
		window.draw(bkGround.shape);
        window.draw(ball.shape);
		window.draw(platform.shape);
        window.display();
    }
	return 0;
}