#pragma once
#include "GrahamScan.h"
#include "Node.h"
#include <queue>
#include <unordered_set>
#include "Waypoint.h"

typedef std::pair<double, Node*> PQelem;

// Fila de prioridade pelo menor primeiro
struct PriorityQueue {
	std::priority_queue<PQelem, std::vector<PQelem>, std::greater<PQelem>> elements;

	inline bool empty() const {
		return elements.empty();
	}

	inline void put(Node* item, double priority) {
		elements.emplace(priority, item);
	}

	Node* get() {
		Node* bestItem = elements.top().second;
		elements.pop();
		return bestItem;
	}

};

namespace std {
	// Sobrecarga de operador para PQelem (prioridade)
	inline bool operator<(PQelem const& a, PQelem const& b) {
		return a.first < b.first;
	}

	inline bool operator>(PQelem const& a, PQelem const& b) {
		return a.first > b.first;
	}

	 //// hash Waypoint
	template <> struct hash<Waypoint> {
		typedef Waypoint argument_type;
		typedef std::size_t result_type;
		std::size_t operator()(const Waypoint& id) const noexcept {
			std::size_t h1 = std::hash<double>{}(id.point.x);
			std::size_t h2 = std::hash<double>{}(id.point.y);
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

	void drawAStarPath(sf::Vector2<double> fromPoint, sf::Vector2<double> toPoint, sf::RenderWindow &window);
		
	void buildAStarPath();

	inline void AStar(Waypoint startPoint, Waypoint goalPoint);

	inline double calcHeuristic(Waypoint w1, Waypoint w2);

	inline double dist(sf::Vertex p, sf::Vertex q);

	inline double dist(sf::Vector2<double> p, sf::Vector2<double> q);

	void drawGraph(sf::RenderWindow &window);

	void clearGraph();

	void clearAStarPath();

	inline friend bool operator==(const sf::Vector2<sf::Vertex>& a, const sf::Vector2<sf::Vertex>& b);
	inline friend bool operator<(const sf::Vector2<double>& a, const sf::Vector2<double>& b);
	inline friend bool operator>(const sf::Vector2<double>& a, const sf::Vector2<double>& b);
	
	inline friend bool operator==(const Waypoint& a, const Waypoint& b);
	inline friend bool operator<(const Waypoint& a, const Waypoint& b);
	inline friend bool operator>(const Waypoint& a, const Waypoint& b); inline friend bool operator==(const sf::Vector2<double>& a, const sf::Vector2<double>& b);
	
	inline friend bool operator==(const Node& a, const Node& b);
private:

	// Linhas que serão desenhadas
	std::vector<sf::Vector2<sf::Vertex>> lines; // Grafo
	std::vector<sf::Vector2<sf::Vertex>> path; // A*
	
	std::vector<Edge*> allEdges;
	std::vector<Cell*> allCells;

	// A*
	std::unordered_set<Waypoint> waypoints; // todos os pontos
	std::vector<Waypoint*> finalPath; // pontos que o A* passa
	std::vector<Node*> nodes; // nodos criados pelo A*

};


bool operator==(const sf::Vector2<sf::Vertex>& a, const sf::Vector2<sf::Vertex>& b) {
	return (a.x == b.x) && (a.y == b.y);
}

bool operator==(const sf::Vector2<double>& a, const sf::Vector2<double>& b) {
	return (a.x == b.x) && (a.y == b.y);
}

bool operator<(const sf::Vector2<double>& a, const sf::Vector2<double>& b) {
	// se ax < bx retorna true. Se for igual, retorna ay < by. Se não, falso.
	return (a.x < b.x) ? true : (a.x == b.x) ? a.y < b.y : false;
}

bool operator>(const sf::Vector2<double>& a, const sf::Vector2<double>& b) {
	return (a.x > b.x) ? true : (a.x == b.x) ? a.y > b.y : false;
}

bool operator==(const Waypoint& a, const Waypoint& b) {
	return a.point == b.point;
}

bool operator>(const Waypoint& a, const Waypoint& b) {
	return a.point > b.point;
}

bool operator<(const Waypoint& a, const Waypoint& b) {
	return a.point < b.point;
}

bool operator==(const Node& a, const Node& b) {
	return a.waypoint == b.waypoint;
}