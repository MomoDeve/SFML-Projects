#include "sfml_extensions.h"

namespace esf
{
	Line::Line() : base(sf::LinesStrip, 2)
	{
		(*this)[0].color = (*this)[1].color = sf::Color::White;
	}

	Line::Line(sf::Vector2f v1, sf::Vector2f v2) : Line()
	{
		(*this)[0].position = v1;
		(*this)[1].position = v2;
	}

	Line::Line(float x1, float y1, float x2, float y2) :
		Line(sf::Vector2f(x1, y1), sf::Vector2f(x2, y2)) { }

	Line::Line(sf::Vector2i v1, sf::Vector2i v2) :
		Line(v1.x, v1.y, v2.x, v2.y) { }

	void Line::setGradientColor(sf::Color c1, sf::Color c2)
	{
		(*this)[0].color = c1;
		(*this)[1].color = c2;
	}

	void Line::setColor(sf::Color color)
	{
		setGradientColor(color, color);
	}



	Grid::Grid(size_t columns, size_t rows, size_t elementSize) :
		size(columns, rows), elementSize(elementSize) { }

	Grid::Grid(sf::Vector2u size, size_t elementSize) :
		size(size), elementSize(elementSize) { }

	void Grid::draw(sf::RenderWindow* window)
	{
		sf::Vector2i yStart(position.x, position.y);
		sf::Vector2i yEnd(yStart.x, yStart.y + elementSize * size.y);
		for (size_t x = 0; x < size.x + 1; x++)
		{
			Line line(yStart, yEnd);
			line.setColor(lineColor);
			window->draw(line);
			yStart.x += elementSize;
			yEnd.x += elementSize;
		}

		sf::Vector2i xStart(position.x, position.y);
		sf::Vector2i xEnd(xStart.x + elementSize * size.x, xStart.y);
		for (size_t y = 0; y < size.y + 1; y++)
		{
			Line line(xStart, xEnd);
			line.setColor(lineColor);
			window->draw(line);
			xStart.y += elementSize;
			xEnd.y += elementSize;
		}
	}

	sf::Vector2u Grid::getSize()
	{
		return size;
	}

	size_t Grid::getColumns()
	{
		return size.x;
	}

	size_t Grid::getRows()
	{
		return size.y;
	}
}
