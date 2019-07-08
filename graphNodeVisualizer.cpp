#include "sfml_extensions.h"
#include <iostream>
#include <algorithm>
#include <map>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <set>

//SFML REQUIRED TO LAUNCH THIS CODE

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define FRAMERATE 60
#define LOG true

using namespace std;

struct node
{
	int id;
	set<int> ribs;
	int posX, posY;
	sf::Color color;
};

using Graph = map<int, node>;

Graph graph;

int main()
{
	sf::Color defaultNodeColor = sf::Color::Yellow;
	const sf::Color selectedNodeColor = sf::Color::Blue;
	srand(time(NULL));
	ifstream file("graph.txt");
	if (file.good())
	{
		int v, u;
		while (file >> v)
		{
			file >> u;
			node& n1 = graph[v];
			node& n2 = graph[u];
			n1.id = v, n2.id = u;
			n1.ribs.insert(u);
			n2.ribs.insert(v);
			n1.color = n2.color = defaultNodeColor;
		}
		if (LOG)
		{
			for (auto const& node : graph)
				cout << "added node: " << node.second.id << endl;
		}
	}
	else
	{
		cout << "error while opening file: graph.txt" << endl;
	}
	file.close();

	int screen_offset = 50;
	for (auto it = graph.begin(); it != graph.end(); it++)
	{
		it->second.posX = screen_offset + rand() % (WINDOW_WIDTH - 2 * screen_offset);
		it->second.posY = screen_offset + rand() % (WINDOW_HEIGHT - 2 * screen_offset);
	}

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Graph Visualizer");
	window.setFramerateLimit(FRAMERATE);

	int circle_size = 30;
	int circle_radius = circle_size / 2;
	node* pressed_node = nullptr;
	node* last_node = nullptr;
	bool connect_mode = false;

	sf::Color lineColor = sf::Color::Red;
	sf::CircleShape shape(circle_size);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonPressed)
			{
				for (auto it = graph.begin(); it != graph.end(); it++)
				{
					sf::Vector2i distVec(it->second.posX - event.mouseButton.x,
										 it->second.posY - event.mouseButton.y);
					int dist = sqrt(distVec.x * distVec.x + distVec.y * distVec.y);
					if (dist <= circle_size)
					{
						pressed_node = &it->second;
						break;
					}
				}
				if (event.mouseButton.button == sf::Mouse::Button::Right)
				{
					if (pressed_node == nullptr)
					{
						node new_node;
						new_node.posX = event.mouseButton.x;
						new_node.posY = event.mouseButton.y;
						new_node.id = rand();
						new_node.color = defaultNodeColor;
						graph.insert({ new_node.id, new_node });
						pressed_node = &graph[new_node.id];
						if (LOG) cout << "added node: " << new_node.id << endl;
					}
					else if (connect_mode)
					{
						if (last_node != nullptr)
						{
							last_node->ribs.insert(pressed_node->id);
							pressed_node->ribs.insert(last_node->id);
							if (LOG) cout << "connecting: " << pressed_node->id << " & " << last_node->id << endl;
						}
					}
					else
					{
						for (auto const& rib : pressed_node->ribs)
						{
							graph[rib].ribs.erase(pressed_node->id);
						}
						int id = pressed_node->id;
						graph.erase(id);
						pressed_node = nullptr;
						if (LOG) cout << "deleted node: " << id << endl;
					}
				}
				if (pressed_node != nullptr) pressed_node->color = selectedNodeColor;
			}
			if (event.type == sf::Event::MouseButtonReleased)
			{
				if(pressed_node != nullptr) pressed_node->color = defaultNodeColor;
				if (connect_mode) last_node = pressed_node;
				pressed_node = nullptr;
			}
			if (event.type == sf::Event::MouseMoved)
			{
				if (pressed_node != nullptr)
				{
					pressed_node->posX = event.mouseMove.x;
					pressed_node->posY = event.mouseMove.y;
				}
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Space)
				{
					if (connect_mode)
					{
						connect_mode = false;
						defaultNodeColor = sf::Color::Yellow;
						if (LOG) cout << ">> insert/delete mode" << endl;
					}
					else
					{
						connect_mode = true;
						defaultNodeColor = sf::Color::Green;
						if (LOG) cout << ">> insert/connect mode" << endl;
					}
					for (auto it = graph.begin(); it != graph.end(); it++)
					{
						it->second.color = defaultNodeColor;
					}
				}
			}
		}
		window.clear();

		for (auto const& node : graph)
		{
			for (auto const& rib : node.second.ribs)
			{
				esf::Line line(node.second.posX, node.second.posY,
							   graph[rib].posX, graph[rib].posY);
				line.setColor(lineColor);
				window.draw(line);
			}
		}

		for (auto const& node: graph)
		{
			shape.setFillColor(node.second.color);
			shape.setPosition(node.second.posX - circle_size, 
							  node.second.posY - circle_size);
			window.draw(shape);
		}

		window.display();
	}
	return 0;
}
