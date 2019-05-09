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
		return size.y;
	}

	size_t Grid::getRows()
	{
		return size.x;
	}
		


	GraphBuilder::GraphBuilder(sf::RenderWindow* window, size_t scaleSize) :
		GraphBuilder(Grid(window->getSize().x / scaleSize, window->getSize().y / scaleSize, scaleSize))
	{
		fillWindow(window);
	}

	GraphBuilder::GraphBuilder(size_t xAxis, size_t yAxis, size_t scaleSize) :
		GraphBuilder(Grid(xAxis, yAxis, scaleSize))
	{

	}

	GraphBuilder::GraphBuilder(Grid grid) :
		grid(grid)
	{
		this->grid.lineColor = sf::Color(255, 255, 255, 100);
		delta = this->grid.elementSize / 10000.0;

		origin = sf::Vector2u(
			grid.position.x + grid.elementSize * (size_t)std::ceil(grid.getRows() / 2.0),
			grid.position.y + grid.elementSize * (size_t)std::ceil(grid.getColumns() / 2.0)
		);
	}
		
	void GraphBuilder::draw(sf::RenderWindow* window)
	{
		if (displayGrid)
		{
			grid.draw(window);
		}

		if (displayAxis)
		{
			const size_t linesCount = 4;
			Line lines[linesCount]
			{
				Line(origin.x, grid.position.y, origin.x, origin.y),
				Line(origin.x, origin.y, origin.x, grid.position.y + grid.elementSize * grid.getColumns()),
				Line(grid.position.x, origin.y, origin.x, origin.y),
				Line(origin.x, origin.y, grid.position.x + grid.elementSize * grid.getRows(), origin.y)
			};

			for (size_t i = 0; i < linesCount; i++)
			{
				lines[i].setColor(AxisColor);
				window->draw(lines[i]);
			}
		}

		for(auto func : functions)
		{
			sf::Vector2u center(getOrigin());
			double dx = -(double)center.x;
			double y = center.y - yScale * func(xScale * dx);
			while (dx < grid.getRows())
			{
				double x = center.x + dx;
				sf::Vector2i lineBegin(grid.position.x + x * grid.elementSize, grid.position.y + y * grid.elementSize);
				dx += delta;
				x += delta;
				y = center.y - yScale * func(xScale * dx);

				sf::Vector2i lineEnd(grid.position.x + x * grid.elementSize, grid.position.y + y * grid.elementSize);
				
				if (lineBegin.x > grid.position.x && 
					lineBegin.y > grid.position.y &&
					lineEnd.x < grid.elementSize * grid.getRows() && 
					lineEnd.y < grid.elementSize * grid.getColumns())
				{
					Line line = Line(lineBegin, lineEnd);
					line.setColor(lineColor);
					window->draw(line);
				}
			}
		}
	}

	sf::Vector2u GraphBuilder::getOrigin()
	{
		size_t x = (origin.x - grid.position.x) / grid.elementSize;
		size_t y = (origin.y - grid.position.y) / grid.elementSize;
		return sf::Vector2u(x, y);
	}

	void GraphBuilder::setOrigin(size_t rows, size_t columns)
	{
		size_t x = std::min(grid.position.x + grid.elementSize * rows, grid.position.x + grid.elementSize * grid.getRows());
		size_t y = std::min(grid.position.y + grid.elementSize * columns, grid.position.y + grid.elementSize * grid.getColumns());
		origin = sf::Vector2u(x, y);
	}

	void GraphBuilder::applyScale(double multiplier)
	{
		yScale *= multiplier;
		xScale *= multiplier;
	}

	void GraphBuilder::zoom(int delta)
	{
		sf::Vector2u origin = getOrigin();
		grid.elementSize = std::max(1, (int)grid.elementSize + delta);
		setOrigin(origin.x, origin.y);
	}

	void GraphBuilder::fillWindow(sf::RenderWindow* window)
	{
		sf::Vector2i windowSize(window->getSize().x, window->getSize().y);

		size_t rows = windowSize.x / grid.elementSize + 1;
		size_t columns = windowSize.y / grid.elementSize + 1;

		Grid newGrid(rows + rows % 2, columns + columns % 2, grid.elementSize);
		newGrid.lineColor = grid.lineColor;

		grid = newGrid;

		origin.x = windowSize.x / 2;
		origin.y = windowSize.y / 2;

		grid.position.x = (windowSize.x - int(grid.elementSize * grid.getRows())) / 2;
		grid.position.y = (windowSize.y - int(grid.elementSize * grid.getColumns())) / 2;
	}
}