#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <iostream>
#include <cmath>

//SFML REQUIRED TO LAUNCH THIS CODE

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define FRAMERATE 120

class Ball : public sf::CircleShape
{
	static float const PI;
	static float const g;
	static float const multiplier;

	float speed_x;
	float speed_y;
	float angle;
public:
	Ball(float radius, int dots = 30)
	{
		sf::CircleShape::setRadius(radius);
		sf::CircleShape::setOrigin(radius, radius);
		angle = PI / 4;
		speed_x = speed_y = 0;
	}

	void push(float x, float y)
	{
		float dy = y - get_y();
		float dx = x - get_x();
		float base_speed = std::sqrt(std::abs(dy) * 2 * g / pow(std::cos(angle), 2) / FRAMERATE);
		update_speed(base_speed);
		if (dx < 0) speed_x *= -1;
		if (dy > 0) speed_y *= -1;
	}

	float get_x()
	{
		return sf::CircleShape::getPosition().x;
	}

	float get_y()
	{
		return sf::CircleShape::getPosition().y;
	}

	void set_pos(float x, float y)
	{
		sf::CircleShape::setPosition(x, y);
	}

	void update()
	{
		if (abs(get_y() - WINDOW_HEIGHT) == getRadius()) speed_x *= multiplier;
		speed_y -= g / FRAMERATE;
		set_pos(get_x() + speed_x, get_y() - speed_y);
		check_collisions();
	}

	void update_speed(float base_speed)
	{
		speed_x = base_speed * std::cos(angle);
		speed_y = base_speed * std::sin(angle);
	}

	void check_collisions()
	{
		float radius = getRadius();

		if (get_y() + radius >= WINDOW_HEIGHT)
		{
			speed_y *= -multiplier;
			set_pos(get_x(), WINDOW_HEIGHT - radius);
		}
		if (get_y() - radius <= 0)
		{
			set_pos(get_x(), radius);
			speed_y *= -multiplier;
		}
		if (get_x() + radius >= WINDOW_WIDTH)
		{
			set_pos(WINDOW_WIDTH - radius, get_y());
			speed_x *= -multiplier;
		}
		if (get_x() - radius <= 0)
		{
			set_pos(radius, get_y());
			speed_x *= -multiplier;
		}
	}
};

float const Ball::PI = 3.1415f;
float const Ball::g = 9.8f;
float const Ball::multiplier = 0.8f;

int main()
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Jumping Ball");
	FreeConsole();
	window.setFramerateLimit(FRAMERATE);

	Ball ball = Ball(30.0f);
	ball.setFillColor(sf::Color::Red);
	ball.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT - ball.getRadius());

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonPressed)
			{
				sf::Vector2i pos = sf::Mouse::getPosition(window);

				ball.push(pos.x, pos.y);
			}
		}

		window.clear();
		window.draw(ball);
		ball.update();
		window.display();
	}
	return 0;
}
