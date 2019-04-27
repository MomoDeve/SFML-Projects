#include "sfml_extensions.h"

//SFML REQUIRED TO LAUNCH THIS CODE

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define FRAMERATE 60

//SFML Extensions Library required
using namespace esf;

int main()
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Graph Builder");
	window.setFramerateLimit(FRAMERATE);

	size_t size = 20;
	double mult = 1.05;

	GraphBuilder builder(&window, size);
	builder.setFunction([](double x) { return x; });
	builder.displayGrid = false;

	GraphBuilder extraBuilder(&window, size);
	extraBuilder.setFunction([](double x) { return x * x; });
	extraBuilder.displayGrid = false;

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
				case sf::Keyboard::Equal:
					builder.applyScale(mult);
					break;
				case sf::Keyboard::Hyphen:
					builder.applyScale(1 / mult);
					break;
				case sf::Keyboard::BackSpace:
					builder.yScale *= -1;
					break;
				case sf::Keyboard::Num0:
					extraBuilder.applyScale(mult);
					break;
				case sf::Keyboard::Num9:
					extraBuilder.applyScale(1 / mult);
					break;
				case sf::Keyboard::Num8:
					extraBuilder.yScale *= -1;
					break;
				}
			}
		}
		window.clear();
		builder.draw(&window);
		extraBuilder.draw(&window);
		window.display();
	}
	return 0;
}
