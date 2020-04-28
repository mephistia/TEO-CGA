#pragma once
#include "GrahamScan.h"
#include "Node.h"

class Graph
{
public:
	Graph();
	~Graph();

	void generateGraph(Diagram diagram);

	double dist(sf::Vertex p, sf::Vertex q);

	void drawGraph(sf::RenderWindow &window);

	void clearGraph();

	inline friend bool operator==(const sf::Vector2<sf::Vertex>& a, const sf::Vector2<sf::Vertex>& b);
private:

	// Linhas que serão desenhadas
	std::vector<sf::Vector2<sf::Vertex>> lines;

};


bool operator==(const sf::Vector2<sf::Vertex>& a, const sf::Vector2<sf::Vertex>& b) {
	return (a.x == b.x) && (a.y == b.y);
}
