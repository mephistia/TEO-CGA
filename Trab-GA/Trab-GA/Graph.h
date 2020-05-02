#pragma once
#include "GrahamScan.h"
#include "Node.h"
#include <queue>
#include <unordered_set>


namespace std {
	// hash vector2<double>
	template <> struct hash<sf::Vector2<double>> {
		typedef sf::Vector2<double> argument_type;
		typedef std::size_t result_type;
		std::size_t operator()(const sf::Vector2<double>& id) const noexcept {
			std::size_t h1 = std::hash<double>{}(id.x);
			std::size_t h2 = std::hash<double>{}(id.y);
			return (h1 ^ (h2 << 1));
		}
	};
}

namespace std {
	// hash node*
	template <> struct hash<Node*> {
		typedef Node* argument_type;
		typedef std::size_t result_type;
		std::size_t operator()(const Node* id) const noexcept {
			std::size_t h1 = std::hash<double>{}(id->point.x);
			std::size_t h2 = std::hash<double>{}(id->point.y);
			return (h1 ^ (h2 << 1));
		}
	};
}

class Graph
{
public:
	Graph();
	~Graph();

	void generateGraph(Diagram diagram);

	void printNodes(Node* ptr, sf::Vector2<double> i);

	Node* createNode(sf::Vector2<double> point, double cost, Node* head, int id);

	Node* getHead(int id);

	Node* getHead(sf::Vector2<double> point);

	std::vector<sf::Vector2<double>> neighbors(sf::Vector2<double> point);
	std::vector<Node*> neighborsN(Node* N);

	void BFS(sf::Vector2<double> startPoint);

	double dist(sf::Vertex p, sf::Vertex q);

	void drawGraph(sf::RenderWindow &window);

	void clearGraph();

	inline friend bool operator<(const sf::Vector2<double>& a, const sf::Vector2<double>& b);
	inline friend bool operator==(const sf::Vector2<sf::Vertex>& a, const sf::Vector2<sf::Vertex>& b);
	inline friend bool cmp(const Node* a, const Node* b);
private:

	// Linhas que serão desenhadas
	std::vector<sf::Vector2<sf::Vertex>> lines;

	// Lista de adjacência
	Node** head;
	
	std::vector<Edge*> allEdges;
	std::vector<Cell*> allCells;

	int nodeId;

};


bool operator==(const sf::Vector2<sf::Vertex>& a, const sf::Vector2<sf::Vertex>& b) {
	return (a.x == b.x) && (a.y == b.y);
}

//bool operator==(const Node& a, const Node& b) {
//	return (a.point == b.point) && (a.next->point == b.next->point);
//}

//bool operator==(const Node& a, const Node& b) {
//	std::cout << "Comparing Nodes... " << "\n";
//	return ((int)a.point.x == (int)b.point.x) && ((int)a.point.y == (int)b.point.y);
//}

bool cmp(const Node* a, const Node* b) {
	std::cout << "Comparing Nodes... " << "\n";
	return ((int)a->point.x == (int)b->point.x) && ((int)a->point.y == (int)b->point.y);
}

template <typename T>
bool operator<(const sf::Vector2<T>& a, const sf::Vector2<T>& b) {
	// se ax < bx retorna true. Se for igual, retorna ay < by. Se não, falso.
	return (a.x < b.x) ? true : (a.x == b.x) ? a.y < b.y : false;
}
