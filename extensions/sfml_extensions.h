#include <SFML/Graphics.hpp>
#pragma once

#ifndef size_t
#define size_t unsigned int
#endif

namespace esf
{
	class Line : public sf::VertexArray
	{
		using base = sf::VertexArray;
	public:
		Line();
		Line(sf::Vector2f v1, sf::Vector2f v2);
		Line(float x1, float y1, float x2, float y2);
		Line(sf::Vector2i v1, sf::Vector2i v2);
		void setGradientColor(sf::Color c1, sf::Color c2);
		void setColor(sf::Color color);
	};



	class Grid
	{
		sf::Vector2u size;
	public:
		sf::Vector2i position = { 0, 0 };
		sf::Color lineColor = sf::Color::White;
		size_t elementSize;

		Grid(size_t columns, size_t rows, size_t elementSize);
		Grid(sf::Vector2u size, size_t elementSize);
		void draw(sf::RenderWindow* window);
		sf::Vector2u getSize();
		size_t getColumns();
		size_t getRows();
	};
}
