#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <list>

//SFML REQUIRED TO LAUNCH THIS CODE

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

class raindrop : public sf::RectangleShape
{
	static const float speed;
public:
	raindrop(float size, float xpos, float ypos)
	{
		sf::RectangleShape::setFillColor(sf::Color::Blue);
		sf::RectangleShape::setSize(sf::Vector2f(size / 8, size));
		sf::RectangleShape::setPosition(xpos, ypos);
	}

	void update()
	{
		sf::Vector2f v = sf::RectangleShape::getPosition();
		v.y += speed;
		sf::RectangleShape::setPosition(v);
	}

	bool outOfBounds(float bound)
	{
		return sf::RectangleShape::getPosition().y > bound;
	}
};

const float raindrop::speed = 10.0f;

class rain
{
	std::list<raindrop> List;
	
	void putDrops()
	{
		int amount = rand() % density;
		for (int i = 0; i < amount; i++)
		{
			raindrop r(rand() % 20, rand() % WINDOW_WIDTH, 0);
			List.emplace_back(r);
		}
	}

public:
	int density;
	rain(int density) : density(density) {}
	
	void update()
	{
		putDrops();
		for (auto it = List.begin(); it != List.end(); it++)
		{
			it->update();
		}
	}
	
	void clear(int bound = WINDOW_HEIGHT)
	{
		for (auto it = List.begin(); it != List.end(); it++)
		{
			if (it->outOfBounds(bound))
			{
				it = List.erase(it);
			}
		}
	}

	void draw(sf::RenderWindow& window)
	{
		for (raindrop r : List) window.draw(r);
	}
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "rainfall");

	rain Rain(1000);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear(sf::Color::White);
		Rain.update();
		Rain.clear(WINDOW_HEIGHT);
		Rain.draw(window);
		window.display();
	}
	return 0;
}
