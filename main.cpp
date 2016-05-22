#include <SFML/Graphics.hpp>
#include <iostream>

#define screenWidth 800
#define screenHight 600
#define ballRadius  6.0
#define platformWidth  (10.0 * ballRadius)  
#define platformHeight (2.0 * ballRadius) 
#define gameFieldWidth (0.50 * screenWidth)
#define gameFieldHight (0.90 * screenHight)
#define fieldBorderX   (0.05 * screenWidth)
#define fieldBorderY   (0.05 * screenHight)

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
	void setSpeedX(float newSpeed){ speed.x = newSpeed; }
	void setSpeedY(float newSpeed){ speed.y = newSpeed; }
	float get_d_speed(){ return d_speed; }
	virtual void update()  = 0;
	virtual float x() = 0;
	virtual float y() = 0;
	virtual float left() = 0;
	virtual float right() = 0;
	virtual float top() = 0;
	virtual float bottom() = 0;
};

template <class firstClass, class secondClass>
bool objectsIntersection(firstClass& first, secondClass& second)
{
    return ((first.right()  >= second.left()) && (first.left() <= second.right())
		 && (first.bottom() >= second.top() ) && (first.top() <= second.bottom()));
}

class Ball : public DinamicVisual{
	bool lose;
public:
	sf::CircleShape shape;
	void setStartPosition()
	{
		float startX = gameFieldWidth / 2.0 + fieldBorderX + platformWidth / 2.0 - ballRadius;
		float startY = gameFieldHight + fieldBorderY - 3.0 * platformHeight;
		shape.setPosition(startX, startY);
	}
	Ball(float ballSpeed)
    {
		setStartPosition();
		lose = false;
		d_speed = ballSpeed;
		speed.x = d_speed;
		speed.y = -d_speed;
        shape.setRadius(ballRadius);
        shape.setFillColor(sf::Color::Yellow);
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
		shape.setPosition(startX, startY - 2 * platformHeight);
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
	sf::Texture texture;
public:
	sf::Sprite sprite;
	//sf::RectangleShape shape;
	BackGround(float startX, float startY)
	{
		if (!texture.loadFromFile("backGround.png"))
			std::cout << "can't find a backGround image\n";
		sprite.setTexture(texture);
		sprite.setPosition(startX, startY);
	};
};

void Ball::update()
{
	if (!lose)
	{
		shape.move(speed);
		//Мячик должен находиться внутри игрового поля
		//Проверки местоположения и последующие настойки скорости
		//горизонтальная скорость
		if (left() < fieldBorderX + d_speed) speed.x = d_speed;
		else if (right() > (fieldBorderX + gameFieldWidth) - d_speed) speed.x = -d_speed;
		//вертикальная скорость
		if (top() < fieldBorderY + d_speed) speed.y = d_speed;
		//можно раскомментировать следущую строчку для бесконечного перемещения шарика внутри поля
		//else if (bottom() > (fieldBorderY + gameFieldHight) - d_speed) speed.y = -d_speed;
		else if (bottom() > (fieldBorderY + gameFieldHight) - d_speed)
		{
			speed.x = 0.0;
			speed.y = 0.0;
			lose = true;
			std::cout << "Game over!\n";
		}
	}
}

void Platform::update()
{
	shape.move(speed);
	//Платформа должена находиться внутри игрового поля
	//Проверка нажатия стрелок клавиатуры для перемещения платфомы
	//Если есть нажание, необходимо изменить скорость
	if( (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) && (left() > fieldBorderX + d_speed))
		speed.x = -d_speed;
	else if((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) && (right() < (fieldBorderX + gameFieldWidth) - d_speed))
			speed.x = d_speed;
		//если нажатия нет или платфома достигла края игрового поля, остановить движение
        else
            speed.x = 0.0;
}

class Collisions{
public:
	void ballAndPlatform(Platform& platformObj, Ball& ballObj)
	{
		//проверяем пересечение мяча и платформы
		if (objectsIntersection(platformObj, ballObj)){
			//необходимо направить мяч вверх
			ballObj.setSpeedY(-1.0 * (ballObj.get_d_speed()));
			//направление по оси x зависит от того, на какое место платформы прилетел мяч
			if (ballObj.x() < platformObj.x()) ballObj.setSpeedX(-1.0 * (ballObj.get_d_speed()));
			                              else ballObj.setSpeedX(ballObj.get_d_speed());
		}
		else return;
	};
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHight), "Arkanoid");
	window.setFramerateLimit(60);
	
	//объекты
	Ball ball{5.0};
	BackGround bkGround{fieldBorderX, fieldBorderY};
	Platform platform{ (gameFieldWidth / 2.0 + fieldBorderX), (fieldBorderY + gameFieldHight), 8.0};
	Collisions collisions;

	sf::Event event;

	bool gameStart = false;

	while(window.isOpen())
    {
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
				break;
			}
		
		}
		
		if (gameStart)
		{
			ball.update();
			platform.update();
			collisions.ballAndPlatform(platform, ball);
		}
		
		window.clear(sf::Color::White);
		window.draw(bkGround.sprite);
        window.draw(ball.shape);
		window.draw(platform.shape);
        window.display();

		while (!gameStart){
			window.pollEvent(event);
			if (event.key.code == sf::Keyboard::Space) gameStart = true;
		}
    }
	return 0;
}