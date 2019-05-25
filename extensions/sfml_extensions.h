#include <SFML/Graphics.hpp>
#include <cmath>
#include <functional>
#include <algorithm>
#pragma once

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



	class GraphBuilder
	{
		Grid grid;
		sf::Vector2i origin;
		double delta;
		sf::RenderWindow* window;
	public:
		sf::Color AxisColor = sf::Color::Red;
		sf::Color lineColor = sf::Color::Green;
		bool displayGrid = true;
		bool displayAxis = true;
		double xScale = 1.0;
		double yScale = 1.0;
		std::vector<std::function<double(double)> > functions;

		GraphBuilder(sf::RenderWindow* window, size_t scaleSize);
		GraphBuilder(sf::RenderWindow* window, size_t xAxis, size_t yAxis, size_t scaleSize);
		GraphBuilder(sf::RenderWindow* window, Grid grid);
		void draw();
		sf::Vector2i getOrigin();
		void setOrigin(int rows, int columns);
		void applyScale(double multiplier);
		void zoom(int delta);
		void move(sf::Vector2i delta);
	};
}