#pragma once
#include "GrahamScan.h"
#include "Node.h"
#include <unordered_map>
#include <queue>

typedef std::pair<double, sf::Vector2<double>> PQelem;

struct PriorityQueue {
	std::priority_queue<PQelem, std::vector<PQelem>, std::greater<PQelem>> elements;

	inline bool empty() const {
		return elements.empty();
	}

	inline void put(sf::Vector2<double> item, double priority) {
		elements.emplace(priority, item);
	}

	sf::Vector2<double> get() {
		sf::Vector2<double> bestItem = elements.top().second;
		elements.pop();
		return bestItem;
	}

};

namespace std {
	// Sobrecarga de operador para PQelem
	inline bool operator<(PQelem const& a, PQelem const& b) {
		return a.first < b.first;
	}

	inline bool operator>(PQelem const& a, PQelem const& b) {
		return a.first > b.first;
	}

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

class Graph
{
public:
	Graph();
	~Graph();


	void generateGraph(Diagram diagram);

	void printNodes(Node* ptr, sf::Vector2<double> i);

	Node* createNode(sf::Vector2<double> point, double cost, Node* head);

	Node* getHead(sf::Vector2<double> point);

	std::vector<Node*> neighborsN(Node* N);

	std::vector<Node*> neighborsN(sf::Vector2<double> N);

	void AStar(sf::Vector2<double> startPoint, sf::Vector2<double> goalPoint);

	inline double dist(sf::Vertex p, sf::Vertex q);

	inline double dist(sf::Vector2<double> p, sf::Vector2<double> q);

	void drawGraph(sf::RenderWindow &window);

	void clearGraph();

	inline friend bool operator==(const sf::Vector2<sf::Vertex>& a, const sf::Vector2<sf::Vertex>& b);
	inline friend bool operator<(const sf::Vector2<double>& a, const sf::Vector2<double>& b);
	inline friend bool operator>(const sf::Vector2<double>& a, const sf::Vector2<double>& b);

private:

	// Linhas que serão desenhadas
	std::vector<sf::Vector2<sf::Vertex>> lines;

	// Lista de adjacência
	Node** head;
	
	std::vector<Edge*> allEdges;
	std::vector<Cell*> allCells;

};


bool operator==(const sf::Vector2<sf::Vertex>& a, const sf::Vector2<sf::Vertex>& b) {
	return (a.x == b.x) && (a.y == b.y);
}


bool operator<(const sf::Vector2<double>& a, const sf::Vector2<double>& b) {
	// se ax < bx retorna true. Se for igual, retorna ay < by. Se não, falso.
	return (a.x < b.x) ? true : (a.x == b.x) ? a.y < b.y : false;
}

bool operator>(const sf::Vector2<double>& a, const sf::Vector2<double>& b) {
	return (a.x > b.x) ? true : (a.x == b.x) ? a.y > b.y : false;
}

