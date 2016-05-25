#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <fstream>

#define screenWidth 800
#define screenHight 600
#define ballRadius  5.0
#define platformWidth  60 
#define platformHeight 10
#define gameFieldWidth (0.50 * screenWidth)
#define gameFieldHight (0.90 * screenHight)
#define fieldBorderX   (0.05 * screenWidth)
#define fieldBorderY   (0.05 * screenHight)
#define brickWidth  60
#define brickHight  15
#define maxInLine   6
#define maxInColumn 18

class Visual{
protected:
	sf::Texture texture;
	sf::FloatRect spriteRect;
public:
	sf::Sprite sprite;
	float x()      { return sprite.getPosition().x; }
	float y()      { return sprite.getPosition().y; }
	float left()   { return x() - spriteRect.width / 2.0; }
	float right()  { return x() + spriteRect.width / 2.0; }
	float top()    { return y() - spriteRect.height / 2.0; }
	float bottom() { return y() + spriteRect.height / 2.0; }
};

class Brick : public Visual{
	int durability;
	bool broken;
public:
	Brick(float startX, float startY, int color = -1, float definedDurability = 0)
	{
		if (color == 0)
		{
			//если цвет в файле задан нулем, тогда на этом месте нет кирпича
			//значит его нужно не учитывать при подсчетах пересечений
			broken = false;
			return;
		}
		if (definedDurability == 0) durability = 1 + std::rand() % 3; //случайное число от 1 до 3
		else durability = definedDurability;
		//int randSprite = 1 + std::rand() % 5; 
		if ((color < 0) || (color > 5)) color = 1 + std::rand() % 5;
		//загрузка необходимой текстуры - разные кирпичики
		switch (color)
		{
		case 1:
			if (!texture.loadFromFile("br_blue.png"))
				std::cout << "can't find a brick image 1\n";
			break;
		case 2:
			if (!texture.loadFromFile("br_red.png"))
				std::cout << "can't find a brick image 2\n";
			break;
		case 3:
			if (!texture.loadFromFile("br_green.png"))
				std::cout << "can't find a brick image 3\n";
			break;
		case 4:
			if (!texture.loadFromFile("br_orange.png"))
				std::cout << "can't find a brick image 3\n";
			break;
		case 5:
			if (!texture.loadFromFile("br_cyan.png"))
				std::cout << "can't find a brick image 3\n";
			break;
		}
		texture.setSmooth(true);
		sprite.setTexture(texture);
		sprite.setTextureRect(sf::IntRect(0, 0, brickWidth, brickHight));
		sprite.setPosition(startX, startY);
		spriteRect = sprite.getGlobalBounds();
		sprite.setOrigin(spriteRect.width / 2.0, spriteRect.height / 2.0);
		broken = false;
	}
	void hit()
	{
		durability--;
		switch (durability)
		{
		case 0:
			broken = true;
			break;
		// если кирпичик не разбит, нужно сдвинуть текстуру, чтобы отобразить трещины
		case 2:
			sprite.setTextureRect(sf::IntRect(brickWidth, 0, brickWidth, brickHight));
			break;
		case 1:
			sprite.setTextureRect(sf::IntRect(2 * brickWidth, 0, brickWidth, brickHight));
			break;
		}
	};
	bool brickBroken() { return broken; }
	void makeInvisible(){ broken = true; }
};

class DinamicVisual : public Visual{
protected:
	sf::Vector2f speed;
	float d_speed;
public:
	virtual void update()  = 0;
	virtual void setStartPosition() = 0;
	void setSpeedX(float newSpeed){ speed.x = newSpeed; }
	void setSpeedY(float newSpeed){ speed.y = newSpeed; }
	void setSpeedXPlus()  { speed.x =  d_speed; }
	void setSpeedXMinus() { speed.x = -d_speed; }
	void setSpeedYPlus()  { speed.y =  d_speed; }
	void setSpeedYMinus() { speed.y = -d_speed; }
	void set_d_speen(float new_d_speed){ d_speed = new_d_speed; }
	float get_d_speed(){ return d_speed; }
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
	virtual void setStartPosition()
	{
		lose = false;
		float startX = gameFieldWidth / 2.0 + fieldBorderX + platformWidth / 2.0 - ballRadius;
		float startY = gameFieldHight + fieldBorderY - 3.0 * platformHeight;
		sprite.setPosition(startX, startY);
	}
	Ball(float ballSpeed)
    {
		d_speed = ballSpeed;
		speed.x = d_speed;
		speed.y = -d_speed;
		if (!texture.loadFromFile("Ball.png"))
			std::cout << "can't find a ball image\n";
		texture.setSmooth(true);
		sprite.setTexture(texture);
		setStartPosition();
		spriteRect = sprite.getGlobalBounds();
		sprite.setOrigin(spriteRect.width / 2.0, spriteRect.height / 2.0);
	};
	bool onGround(){ return lose; }
	virtual void update()
	{
		if (!lose)
		{
			sprite.move(speed);
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
	};
};

class Platform : public DinamicVisual{
public:
	virtual void setStartPosition()
	{
		float startX = (gameFieldWidth / 2.0) + fieldBorderX;
		float startY = (fieldBorderY + gameFieldHight) - 2 * platformHeight;
		sprite.setPosition(startX, startY);
	};
	Platform(float platformSpeed)
	{
		d_speed = platformSpeed;
		speed.x = 0.0;
		speed.y = 0.0;
		if (!texture.loadFromFile("Platform.png"))
			std::cout << "can't find a platform image\n";
		texture.setSmooth(true);
		sprite.setTexture(texture);
		sprite.setTextureRect(sf::IntRect(0, 0, platformWidth, platformHeight));
		setStartPosition();
		spriteRect = sprite.getGlobalBounds();
		sprite.setOrigin(spriteRect.width / 2.0, spriteRect.height / 2.0);
	};
	virtual void update()
	{
		sprite.move(speed);
		//Платформа должна находиться внутри игрового поля
		//Проверка нажатия стрелок клавиатуры для перемещения платфомы
		//Если есть нажание, необходимо изменить скорость
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) && (left() > fieldBorderX + d_speed))
			speed.x = -d_speed;
		else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) && (right() < (fieldBorderX + gameFieldWidth) - d_speed))
			speed.x = d_speed;
		//если нажатия нет или платфома достигла края игрового поля, остановить движение
		else
			speed.x = 0.0;
	};
};

class BackGround : public sf::Drawable{
	sf::Texture gameFieldT;
	sf::Texture mainBkGroundT;
	sf::Sprite gameField;
	sf::Sprite mainBkGround;
public:
	BackGround()
	{
		if (!gameFieldT.loadFromFile("backGround.png"))
			std::cout << "can't find a game field image\n";
		gameField.setTexture(gameFieldT);
		gameField.setPosition(fieldBorderX, fieldBorderY);
		if (!mainBkGroundT.loadFromFile("MainBk.png"))
			std::cout << "can't find a main back ground image\n";
		mainBkGround.setTexture(mainBkGroundT);
		mainBkGround.setPosition(0, 0);
	};
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(mainBkGround, states);
		target.draw(gameField, states);
	};
};

//класс, проверяющий столкновения объектов
class Collisions{
public:
	void ballAndPlatform(Platform& platformObj, Ball& ballObj)
	{
		//проверяем пересечение мяча и платформы
		if (objectsIntersection(platformObj, ballObj)){
			//необходимо направить мяч вверх
			ballObj.setSpeedY(-1.0 * (ballObj.get_d_speed()));
			//направление по оси x зависит от того, на какое место платформы прилетел мяч
			if (ballObj.x() < platformObj.x()) ballObj.setSpeedXMinus();
			                              else ballObj.setSpeedXPlus();
		}
		else return;
	};
	void ballAndBrick(Brick& brickObj, Ball& ballObj)
	{
		//проверяем было ли пересечение мяча с кирпичиком
		if(!objectsIntersection(brickObj, ballObj)) return;
		//в случае пересечния - удар
		brickObj.hit();
		//необходимо выяснить с какой стороны мяч сильнее "наехал" на кирпич
		//дельты пересечений со всех стороны
		float deltaLeft   = abs(ballObj.right()  - brickObj.left()  );
		float deltaRight  = abs(ballObj.left()   - brickObj.right() );
		float deltaTop    = abs(ballObj.bottom() - brickObj.top()   );
		float deltaBottom = abs(ballObj.top()    - brickObj.bottom());
		bool leftDir = (deltaLeft < deltaRight);
		bool topDir  = (deltaTop  < deltaBottom);
		float minDeltaX = (leftDir) ? deltaLeft : deltaRight;
		float minDeltaY = (topDir ) ? deltaTop  : deltaBottom;
		//значение дельты указывает на направление удара - горизонтальное или вертикальное
		//если горизотальная дельта больше - значит мяч летел вертикально и пересек границы кирпича
		//в ином случае - мяч прилетел в большей степени по горизонтали
		if (minDeltaX > minDeltaY) //горизонтальное напрвление
			if (topDir) ballObj.setSpeedYMinus();
			       else ballObj.setSpeedYPlus();
		else //вертикальный удар
			if (leftDir) ballObj.setSpeedXMinus();
			        else ballObj.setSpeedXPlus();
	};
};

class Life : public Visual{
	int lifeNumber;
public:
	Life()
	{
		lifeNumber = 3;
		if (!texture.loadFromFile("Life.png"))
			std::cout << "can't find a life image\n";
		texture.setSmooth(true);
		sprite.setTexture(texture);
		spriteRect = sprite.getGlobalBounds();
		sprite.setPosition((screenWidth - 20), fieldBorderY / 2.0);
	};
	bool noLife(){ return (lifeNumber == 0) ? true : false; }
	void newLvl()
	{
		lifeNumber = 3;
		sprite.setTextureRect(sf::IntRect(0, 0, 180, 60));
		sprite.setOrigin(spriteRect.width, 0);
	};
	void ballFall()
	{
		lifeNumber--;
		std::cout << "life: "<< lifeNumber << std::endl;
		if (lifeNumber != 0)
		{
			sprite.setTextureRect(sf::IntRect(0, 0, (lifeNumber * 60), 60));
			sprite.setOrigin(60 * lifeNumber, 0);
		}
		else sprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
	};
};

class Game{
	Life life;
	sf::Vector2f allBricksCoordinates[maxInLine][maxInColumn];
	int lvlMap[maxInLine][maxInColumn];
public:
	std::vector<Brick> bricks;
	Game()
	{
		for (int j = 0; j < maxInColumn; j++)
			for (int i = 0; i < maxInLine; i++)
			{
				//заранее вычислим все координаты всех возможных кирпичей и поместим их в матрицу
				allBricksCoordinates[i][j].x = (i + 1) * (brickWidth + 4) + fieldBorderX + ((gameFieldWidth - 4 - (brickWidth + 4) * maxInLine) / 2.0) - brickWidth / 2.0;
				allBricksCoordinates[i][j].y = (j + 1) * (brickHight + 2) + fieldBorderY - brickHight / 2.0;
			}
	};
	void uploadLvlMap(char lvl[10])
	{
		std::ifstream input;
		input.open(lvl);
		if (!input.is_open()) // если файл не открыт
			std::cout << "Карта уровня не найдена!\n"; // сообщить об этом
		else
		{
			for (int j = 0; j < maxInColumn; j++)
				for (int i = 0; i < maxInLine; i++)
				{
					input >> lvlMap[i][j];
					std::cout << lvlMap[i][j] << " ";
					if (i == (maxInLine - 1)) std::cout << std::endl;
				}
			input.close();
		}
	};
	void newLvl(char lvlFile[10])
	{
		//в дальнейшем в этой функции будет производиться считывание карты расстановки кирпичей
		uploadLvlMap(lvlFile);
		//-----------------------------------------------------------------------------------------
		//если в конструкторе не задана прочность и цвет кирпича, они выбираются случайным способом
		//поэтому необходимо использовать srand перед генерацией кирпичей
		srand(time(NULL));
		for (int j = 0; j < maxInColumn; j++)
			for (int i = 0; i < maxInLine; i++)
			{
				Brick* newBrick = new Brick(allBricksCoordinates[i][j].x, allBricksCoordinates[i][j].y, lvlMap[i][j]);
				bricks.push_back(*newBrick);
			}
	};
	void lvlOver()
	{
		bricks.clear();
	};
	void GameLoop();
	void lvlLoop(sf::RenderWindow &openedWindow, BackGround& bkGr, float ballStartSpeed)
	{
		Ball ball{ ballStartSpeed };
		Platform platform{ 8.0 };
		Collisions collisions;
		newLvl("lvl_01.txt");
		life.newLvl();
		bool play = true;
		bool ballFly = false;
		sf::Event event;

		while (play)
		{
			while (openedWindow.pollEvent(event))
			{
				switch (event.type)
				{
				case sf::Event::Closed:
					openedWindow.close();
					break;
				case sf::Event::KeyPressed:
					if (event.key.code == sf::Keyboard::Escape)
						openedWindow.close();
						//здесь будет возврат в будущее меню
					break;
				}
			}

			if (ballFly)
			{
				ball.update();
				platform.update();
				collisions.ballAndPlatform(platform, ball);
				//проверка столкновения мяча с кирпичиками
				for (int i = 0; i < bricks.size(); i++)
					if (!bricks[i].brickBroken()) collisions.ballAndBrick(bricks[i], ball);
			}

			openedWindow.clear(sf::Color::Black);
			openedWindow.draw(bkGr);
			openedWindow.draw(platform.sprite);
			//прорисовка вектора кирпичей
			for (int i = 0; i < bricks.size(); i++)
				if (!bricks[i].brickBroken()) openedWindow.draw(bricks[i].sprite);
			openedWindow.draw(ball.sprite);
			openedWindow.draw(life.sprite);

			openedWindow.display();

			while (!ballFly){
				openedWindow.pollEvent(event);
				if (event.key.code == sf::Keyboard::Space) ballFly = true;
				if (event.key.code == sf::Keyboard::Escape) openedWindow.close(); //сделать выход в меню!
			}
			//если мяч упал, необходимо отнять жизнь
			if (ball.onGround())
			{
				ballFly = false;
				life.ballFall();
				//если еще есть жизни, вернуть мяч на начальную позицию
				if (!life.noLife())
				{
					ball.setStartPosition();
					platform.setStartPosition();
				}
				else
				{
					play = false;
					lvlOver();
				}
			}
		}
	};
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHight), "Arkanoid");
	window.setFramerateLimit(60);
	
	//объекты
	BackGround bkGround;
	Game game;

	sf::Event event;

	while (window.isOpen())
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
		window.clear(sf::Color::Black);
		window.draw(bkGround);

		game.lvlLoop(window, bkGround, 4.0);
	}
	return 0;
}