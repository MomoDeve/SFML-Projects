#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <list>
#include <exception>
#include <algorithm>

//SFML REQUIRED TO LAUNCH THIS CODE

#define WINDOW_WIDTH 480
#define WINDOW_HEIGHT 720

class Player;
class Enemy;
class Bullet;

class GameCore
{
	sf::RenderWindow* window;
	Player* player;
	std::list<Enemy> enemyList;

	void updateBullets();

	void updateEnemyMovement();
public:
	std::list<Bullet> playerBullets, enemyBullets;

	GameCore(sf::RenderWindow* window);

	virtual void startGame();

	virtual void addPlayer(Player* player);

	virtual void draw(sf::RenderWindow& window);

	virtual void update(sf::Event& e);

	virtual void update();

	virtual void generateEnemies();

	virtual void reset();

};

class gameException : public std::exception
{
public:
	gameException(const char* message) : std::exception(message) { }
};

class Entity : public sf::RectangleShape
{
	using base = sf::RectangleShape;
public:

	Entity(sf::Vector2f position, sf::Vector2f size) : base(size)
	{
		base::setPosition(position.x, position.y);
	}

	virtual void setColor(sf::Color color)
	{
		base::setFillColor(color);
	}

	virtual float getX()
	{
		return base::getPosition().x;
	}

	virtual float getY()
	{
		return base::getPosition().y;
	}

	virtual void setX(float x)
	{
		base::setPosition(x, getY());
	}

	virtual void setY(float y)
	{
		base::setPosition(getX(), y);
	}

	virtual void update() { }

	virtual void destroy() { }

	virtual bool checkCollision(Entity& e) { return false; }
};

class Bullet : public Entity
{
	float speed;
public:
	Bullet(sf::Vector2f position, sf::Vector2f size, float speed) : Entity(position, size)
	{
		this->speed = speed;
	}

	virtual void update() override
	{
		setY(getY() + speed);
	}

	virtual bool checkCollision(Entity& e) override
	{
		return e.getX() < getX() + getSize().x &&
			   e.getX() + e.getSize().x > getX() &&
			   e.getY() > getY() - getSize().y &&
			   e.getY() - e.getSize().y < e.getY();
	}
};

class Player : public Entity
{
	GameCore* core;
	using Key = sf::Keyboard::Key;
public:
	float speed;

	Player(sf::Vector2f position, sf::Vector2f size, float speed, GameCore* core) : Entity(position, size)
	{
		this->core = core;
		this->speed = speed;
	}

	virtual void update(Key k)
	{
		float x = getX(), y = getY();
		if(k ==  Key::A)
			setX(x - speed);
		if(k == Key::D)
			setX(x + speed);
		if(k == Key::Space)
			shoot();
	}

	virtual void shoot()
	{
		float xsize = 5, ysize = 20, speed = -2.0f;
		sf::Vector2f pos(getX() + getSize().x / 2, getY() - ysize);
		core->playerBullets.emplace_back(Bullet(pos, sf::Vector2f(xsize, ysize), speed));
	}

	virtual void checkWallCollision(sf::Vector2u size)
	{
		float x = getX(), xSize = getSize().x, border = size.x;
		if (x + xSize > border)
			setX(border - xSize);
		else if (x < 0.0f)
			setX(0.0f);
	}

	virtual bool checkCollision(Entity& e) override
	{
		return e.getX() < getX() + getSize().x &&
			e.getX() + e.getSize().x > getX() &&
			e.getY() + e.getSize().y > getY() &&
			e.getY() < e.getY() + e.getSize().y;
	}
};

class Enemy : public Entity
{
public:
	float speed;

	Enemy(sf::Vector2f position, sf::Vector2f size, float speed) : Entity(position, size)
	{
		this->speed = speed;
	}

	virtual void reverseMovement()
	{
		speed *= -1;
	}

	virtual void update() override
	{
		setX(getX() + speed);
	}

	virtual void shoot(GameCore* core)
	{	
		if (rand() % 300 != 0) return;
		float xsize = 5, ysize = 20, speed = 4.0f;
		sf::Vector2f pos(getX() + getSize().x / 2, getY() - ysize);
		core->enemyBullets.emplace_back(Bullet(pos, sf::Vector2f(xsize, ysize), speed));
	}
};

int main()
{
	using v2f = sf::Vector2f;
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Space Invaders", sf::Style::Titlebar | sf::Style::Close);
	window.setFramerateLimit(60);
	FreeConsole();

	float speed = 16.0f;
	float width = 25, height = 50, initX = WINDOW_WIDTH * 0.4f, initY = WINDOW_HEIGHT * 0.9f;
	GameCore core(&window);
	Player player(v2f(initX, initY), v2f(height, width), speed, &core);
	player.setColor(sf::Color::Red);
	core.addPlayer(&player);
	core.startGame();

	sf::Clock clock;
	bool allowShooting = true;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (clock.getElapsedTime().asSeconds() > 1.0f)
			{
				clock.restart();
				allowShooting = true;
			}

			if (event.key.code != sf::Keyboard::Space)
			{
				core.update(event);
			}
			else if (allowShooting)
			{
				core.update(event);
				allowShooting = false;
			}
		}
		core.update();
		window.clear();
		core.draw(window);
		window.display();
	}
	return 0;
}

void GameCore::updateBullets()
{
	for (auto it = playerBullets.begin(); it != playerBullets.end(); it++)
	{
		it->update();
		if (it->getY() + it->getSize().y < 0)
		{
			it = playerBullets.erase(it);
		}
		else
		{
			for (auto e = enemyList.begin(); e != enemyList.end(); e++)
			{
				if (it->checkCollision(*e))
				{
					enemyList.erase(e);
					it = playerBullets.erase(it);
					break;
				}
			}
		}
		if (it == playerBullets.end()) break;
	}

	for (auto it = enemyBullets.begin(); it != enemyBullets.end(); it++)
	{
		it->update();
		if (it->getY() > window->getSize().y)
		{
			it = enemyBullets.erase(it);
			if (it == enemyBullets.end()) return;
		}
		else
		{
			if (player->checkCollision(*it))
			{
				reset();
				return;
			}					
		}
	}
}

void GameCore::updateEnemyMovement()
{
	if (enemyList.empty())
		reset();

	bool reverse = false;
	for (auto e = enemyList.begin(); e != enemyList.end(); e++)
	{
		if (e->getX() <= 0 || e->getX() + e->getSize().x >= window->getSize().x)
		{
			reverse = true;
			break;
		}
	}

	if (reverse)
	{
		for (auto it = enemyList.begin(); it != enemyList.end(); it++)
		{
			it->reverseMovement();
		}
	}
}

GameCore::GameCore(sf::RenderWindow* window)
{
	this->window = window;
}

void GameCore::startGame()
{
	if (player == nullptr) throw gameException("Player has not been created");
	generateEnemies();
}

void GameCore::addPlayer(Player* player)
{
	this->player = player;
}

void GameCore::draw(sf::RenderWindow& window)
{
	window.draw(*player);
	for (Enemy enemy : enemyList)
		window.draw(enemy);
	for (Bullet bullet : playerBullets)
		window.draw(bullet);
	for (Bullet bullet : enemyBullets)
		window.draw(bullet);
}

void GameCore::update(sf::Event& e)
{
	if (e.type == sf::Event::KeyPressed)
	{
		player->update(e.key.code);
		player->checkWallCollision(window->getSize());
	}
}

void GameCore::update()
{
	updateBullets();

	for (auto it = enemyList.begin(); it != enemyList.end(); it++)
	{
		it->update();
		it->shoot(this);
	}

	updateEnemyMovement();
}

void GameCore::generateEnemies()
{
	float xsize = 30, ysize = 20, speed = 1.0f;
	int width = window->getSize().x, height = window->getSize().y;
	float y = height / 70;
	for (int i = 0; i < 3; i++)
	{
		float x = width / 35;
		for (int j = 0; j < 4; j++)
		{
			enemyList.emplace_back(Enemy(sf::Vector2f(x, y), sf::Vector2f(xsize, ysize), speed));
			x += width / 4;
		}
		y += height / 10;
	}
}

void GameCore::reset()
{
	enemyList.clear();
	generateEnemies();
	enemyBullets.clear();
	playerBullets.clear();
}
