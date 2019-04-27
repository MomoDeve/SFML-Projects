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
		sf::Vector2u origin;
		std::function<double(double)> func;
		bool functionSet = false;
		double delta = 0.001;
	public:
		sf::Color AxisColor = sf::Color::Red;
		bool displayGrid = true;
		bool displayAxis = true;
		double xScale = 1.0;
		double yScale = 1.0;

		GraphBuilder(sf::RenderWindow* window, size_t scaleSize);
		GraphBuilder(size_t xAxis, size_t yAxis, size_t scaleSize);
		GraphBuilder(Grid grid);
		void draw(sf::RenderWindow* window);
		sf::Vector2u getOrigin();
		void setOrigin(size_t rows, size_t columns);
		void setFunction(std::function<double(double)> f);
		void applyScale(double multiplier);
	};
}
