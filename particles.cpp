#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <iostream>
#include <cmath>
#include <list>

//SFML REQUIRED TO LAUNCH THIS CODE

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define FRAMERATE 120

static const float PI = 3.1415f;

class Particle : public sf::CircleShape
{
	using base = sf::CircleShape;
	float speed;
	float angle;

public:
	Particle(float size, float speed, float angle = 0.0f) : base(size, 15)
	{
		base::setOrigin(size / 2, size / 2);
		base::setFillColor(sf::Color::Yellow);
		this->speed = speed;
		this->angle = angle;
	}

	inline float getX()
	{
		return base::getPosition().x;
	}

	inline float getY()
	{
		return base::getPosition().y;
	}

	inline void setX(float x)
	{
		base::setPosition(x, getY());
	}

	inline void setY(float y)
	{
		base::setPosition(getX(), y);
	}

	void update()
	{
		float dx = speed * std::cos(angle);
		float dy = speed * std::sin(angle);
		setX(getX() + dx);
		setY(getY() + dy);
	}

	void checkWallCollision()
	{
		float radius = getRadius();

		if (getY() + radius >= WINDOW_HEIGHT)
		{
			angle *= -1;
			setY(WINDOW_HEIGHT - radius);
		}
		if (getY() - radius <= 0)
		{
			angle *= -1;
			setY(radius);
		}
		if (getX() + radius >= WINDOW_WIDTH)
		{
			angle = PI - angle;
			setX(WINDOW_WIDTH - radius);
		}
		if (getX() - radius <= 0)
		{
			angle = PI - angle;
			setX(radius);
		}
	}

	void checkCollision(Particle& p)
	{
		if(std::abs(getX() - p.getX()) <= getRadius() &&
		   std::abs(getY() - p.getY()) <= getRadius())
		{
			angle = -PI + angle;
			update();
		}
	}

	void redirect(float dx, float dy)
	{
		float newAngle = std::atan(std::abs(dy) / std::abs(dx));
		if (dx < 0 && dy > 0)
		{
			newAngle = PI - newAngle;
		}
		else if (dy < 0 && dx < 0)
		{
			newAngle = PI + newAngle;
		}
		else if (dy < 0 && dx > 0)
		{
			newAngle *= -1;
		}
		angle = newAngle;
	}
};

class Core
{
	sf::RenderWindow* window;
	std::list<Particle> particles;
	const float particleSize = 2;
	const int particleInitSpeed = 20;
	const int particleAmount = 100;

	void generateParticles()
	{
		for (int i = 0; i < particleAmount; i++)
		{
			Particle p(particleSize, rand() % particleInitSpeed + particleInitSpeed / 2, rand());
			p.setX(rand() % WINDOW_WIDTH);
			p.setY(rand() % WINDOW_HEIGHT);
			particles.push_back(p);
		}
	}
public:
	Core(sf::RenderWindow* window)
	{
		this->window = window;
	}
	void drawParticles()
	{	
		for (auto it = particles.begin(); it != particles.end(); it++)
		{
			for (auto other = particles.begin(); other != particles.end(); other++)
			{
				if (it != other)
				{
					it->checkCollision(*other);
				}
			}
		}

		for (auto it = particles.begin(); it != particles.end(); it++)
		{
			it->checkWallCollision();
			it->update();
			window->draw(*it);
		}
			
	}

	void push(float x, float y)
	{
		for (auto it = particles.begin(); it != particles.end(); it++)
		{
			it->redirect(x - it->getX(), y - it->getY());
		}
	}

	void start()
	{
		generateParticles();
	}
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Particles");
	FreeConsole();
	window.setFramerateLimit(FRAMERATE);
	Core core(&window);
	core.start();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseMoved)
			{
				sf::Vector2i pos = sf::Mouse::getPosition(window);
				core.push(pos.x, pos.y);
			}
		}
		window.clear();
		core.drawParticles();
		window.display();
	}
	return 0;
}
