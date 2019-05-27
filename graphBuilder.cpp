#include "sfml_extensions.h"

//SFML REQUIRED TO LAUNCH THIS CODE

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define FRAMERATE 60

//SFML Extensions Library required
using namespace esf;
using namespace std;

class GraphBuilderWrapper
{
	sf::Vector2i mousePos;
public:
	GraphBuilder* builder;
	int zoomDelta;
	int moveDelta;
	double ScaleDelta;
	bool mousePressed;

	GraphBuilderWrapper(esf::GraphBuilder* builder) : builder(builder)
	{

	}
	void draw()
	{
		builder->draw();
	}
	void checkKeyEvent(sf::Keyboard::Key key)
	{
		switch (key)
		{
		case sf::Keyboard::Num0:
			builder->xScale *= ScaleDelta;
			break;
		case sf::Keyboard::Num9:
			builder->xScale *= 1 / ScaleDelta;
			break;
		case sf::Keyboard::Num8:
			builder->applyScale(ScaleDelta);
			break;
		case sf::Keyboard::Num7:
			builder->applyScale(1 / ScaleDelta);
			break;
		case sf::Keyboard::Equal:
			builder->zoom(zoomDelta);
			break;
		case sf::Keyboard::Hyphen:
			builder->zoom(-zoomDelta);
			break;
		case sf::Keyboard::BackSpace:
			builder->yScale *= -1;
			break;
		case sf::Keyboard::A:
			builder->move(sf::Vector2i(moveDelta, 0));
			break;
		case sf::Keyboard::W:
			builder->move(sf::Vector2i(0, moveDelta));
			break;
		case sf::Keyboard::S:
			builder->move(sf::Vector2i(0, -moveDelta));
			break;
		case sf::Keyboard::D:
			builder->move(sf::Vector2i(-moveDelta, 0));
			break;
		}
	}
	void checkMouseEvent(sf::Event::MouseMoveEvent mouse)
	{
		sf::Vector2i newMousePos(mouse.x, mouse.y);
		if (mousePressed)
		{
			builder->move(newMousePos - mousePos);
		}
		mousePos = newMousePos;
	}
	void wheelMoved(sf::Event::MouseWheelEvent wheel)
	{
		if (wheel.delta > 0)
		{
			builder->zoom(zoomDelta);
		}
		else
		{
			builder->zoom(-zoomDelta);
		}
	}
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Graph Builder");
	window.setFramerateLimit(FRAMERATE);

	size_t size = 100;

	GraphBuilder builder(&window, 100, 100, size);
	builder.functions.emplace_back([](double x) { return x * sin(x); });
	builder.displayGrid = true;
	builder.lineColor = sf::Color::Cyan;

	GraphBuilderWrapper wrapper(&builder);
	wrapper.moveDelta = 20;
	wrapper.ScaleDelta = 1.05;
	wrapper.zoomDelta = 2;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed)
			{
				wrapper.checkKeyEvent(event.key.code);
			}
			if (event.type == sf::Event::MouseMoved)
			{
				wrapper.checkMouseEvent(event.mouseMove);
			}
			if (event.type == sf::Event::MouseWheelMoved)
			{
				wrapper.wheelMoved(event.mouseWheel);
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				wrapper.mousePressed = true;
			}
			else if (event.type == sf::Event::MouseButtonReleased)
			{
				wrapper.mousePressed = false;
			}
		}
		window.clear();
		wrapper.draw();
		window.display();
	}
	return 0;
}
