#include "sfml_extensions.h"

//SFML REQUIRED TO LAUNCH THIS CODE

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define FRAMERATE 60

//SFML Extensions Library required
using namespace esf;
using namespace std;

int main()
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Graph Builder");
	window.setFramerateLimit(FRAMERATE);

	size_t size = 100;
	int zoom = 1;
	double scale = 1.05;

	GraphBuilder builder(&window, size);
	builder.functions.emplace_back([](double x) { return sin(x) * x; });
	builder.displayGrid = true;
	builder.lineColor = sf::Color::Cyan;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::Num0:
					builder.xScale *= scale;
					break;
				case sf::Keyboard::Num9:
					builder.xScale *= 1 / scale;
					break;
				case sf::Keyboard::Equal:
					builder.zoom(zoom);
					//builder.fillWindow(&window);
					break;
				case sf::Keyboard::Hyphen:
					builder.zoom(-zoom);
					//builder.fillWindow(&window);
					break;
				case sf::Keyboard::BackSpace:
					builder.yScale *= -1;
					break;
				}
			}
		}
		window.clear();
		builder.draw(&window);
		window.display();
	}
	return 0;
}
