#pragma once
#include "GrahamScan.h"
#include "AStarNode.h"
//#include <unordered_set>
#include <queue>

typedef std::pair<double, AStarNode*> PQelem;

struct PriorityQueue {
	std::priority_queue<PQelem, std::vector<PQelem>, std::greater<PQelem>> elements;

	inline bool empty() const {
		return elements.empty();
	}

	inline void put(AStarNode* item, double priority) {
		elements.emplace(priority, item);
	}

	AStarNode* get() {
		AStarNode* bestItem = elements.top().second;
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

	// hash Node
	//template <> struct hash<Node> {
	//	typedef sf::Vector2<double> argument_type;
	//	typedef std::size_t result_type;
	//	std::size_t operator()(const Node id) const noexcept {
	//		std::size_t h1 = std::hash<double>{}(id.point.x);
	//		std::size_t h2 = std::hash<double>{}(id.point.y);
	//		return (h1 ^ (h2 << 1));
	//	}
	//};
}

struct mHashFunc {
	size_t operator()(const Node& n) const {
		return (std::hash<double>()(n.point.x)) ^ (std::hash<double>()(n.point.y));
	}
};

struct mEdge {
	sf::Vector2<double> src, dest;
};


class Graph
{
public:
	Graph();
	~Graph();


	void generateGraph(Diagram diagram);

	inline void buildNeighbors();

	inline Node getHead(sf::Vector2<double> point);

	inline void AStar(sf::Vector2<double> startPoint, sf::Vector2<double> goalPoint);

	inline void buildAStarPath();

	void drawAStarPath(sf::Vector2<double> fromPoint, sf::Vector2<double> toPoint, sf::RenderWindow& window);

	inline double dist(sf::Vertex p, sf::Vertex q);

	inline double dist(sf::Vector2<double> p, sf::Vector2<double> q);

	void drawGraph(sf::RenderWindow &window);

	void clearGraph();

	void clearAStarPath();

	inline friend bool operator==(const sf::Vector2<sf::Vertex>& a, const sf::Vector2<sf::Vertex>& b);
	inline friend bool operator<(const sf::Vector2<double>& a, const sf::Vector2<double>& b);
	inline friend bool operator>(const sf::Vector2<double>& a, const sf::Vector2<double>& b);

	inline friend bool operator==(const Node& a, const Node& b);
	inline friend bool operator<(const Node& a, const Node& b);
	inline friend bool operator>(const Node& a, const Node& b);
private:

	// Linhas que serão desenhadas
	std::vector<sf::Vector2<sf::Vertex>> lines; // Grafo
	std::vector<sf::Vector2<sf::Vertex>> path; // A*


	std::vector<Edge*> allEdges;
	std::vector<Cell*> allCells;

	// A*
	std::vector<mEdge> allmEdges;
	std::vector<AStarNode*> ANodes;
	std::vector<Node> finalPath;
	std::unordered_set<Node, mHashFunc> allPoints;

	bool AStarDone;

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

bool operator==(const Node& a, const Node& b) { return a.point == b.point; }
bool operator<(const Node& a, const Node& b) { return a.point < b.point; }
bool operator>(const Node& a, const Node& b) { return a.point > b.point; }