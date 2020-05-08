#pragma once
#include "GrahamScan.h"
#include "AStarNode.h"
#include <queue>
#include <unordered_set>

typedef std::pair<double, AStarNode*> PQelem;

namespace std {
	// Sobrecarga de operador para PQelem
	inline bool operator<(PQelem const& a, PQelem const& b) {
		return a.first < b.first;
	}

	inline bool operator>(PQelem const& a, PQelem const& b) {
		return a.first > b.first;
	}
}


struct mHashFunc {
	size_t operator()(const Node& n) const {
		return (std::hash<int>()(n.point.x)) ^ (std::hash<int>()(n.point.y));
	}
};

struct mEdge {
	sf::Vector2i src, dest;
};


class Graph
{
public:
	Graph();
	~Graph();


	void generateGraph(Diagram diagram);

	inline Node getHead(sf::Vector2i point);

	inline void AStar(sf::Vector2i startPoint, sf::Vector2i goalPoint);

	inline std::vector<AStarNode*> getNeighbors(AStarNode* n);

	inline void buildAStarPath();

	void drawAStarPath(sf::Vector2i fromPoint, sf::Vector2i toPoint, sf::RenderWindow& window);

	inline double dist(sf::Vertex p, sf::Vertex q);

	inline double dist(sf::Vector2i p, sf::Vector2i q);

	void drawGraph(sf::RenderWindow &window);

	void clearGraph();

	void clearAStarPath();

	inline friend bool operator==(const sf::Vector2<sf::Vertex>& a, const sf::Vector2<sf::Vertex>& b);
	inline friend bool operator<(const sf::Vector2i& a, const sf::Vector2i& b);
	inline friend bool operator>(const sf::Vector2i& a, const sf::Vector2i& b);

	inline friend bool operator==(const Node& a, const Node& b);
	inline friend bool operator!=(const Node& a, const Node& b);
	inline friend bool operator<(const Node& a, const Node& b);
	inline friend bool operator>(const Node& a, const Node& b);
	
	inline friend bool operator==(const AStarNode& a, const AStarNode& b);
	inline friend bool operator!=(const AStarNode& a, const AStarNode& b);
	inline friend bool operator<(const AStarNode& a, const AStarNode& b);
	inline friend bool operator>(const AStarNode& a, const AStarNode& b);
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
	std::set<PQelem, std::less<PQelem>> pq; // ou aStarHash

	bool AStarDone;

};


bool operator==(const sf::Vector2<sf::Vertex>& a, const sf::Vector2<sf::Vertex>& b) {
	return (a.x == b.x) && (a.y == b.y);
}


bool operator<(const sf::Vector2i& a, const sf::Vector2i& b) {
	// se ax < bx retorna true. Se for igual, retorna ay < by. Se não, falso.
	return (a.x < b.x) ? true : (a.x == b.x) ? a.y < b.y : false;
}

bool operator>(const sf::Vector2i& a, const sf::Vector2i& b) {
	return (a.x > b.x) ? true : (a.x == b.x) ? a.y > b.y : false;
}

bool operator==(const Node& a, const Node& b) { return a.point == b.point; }
bool operator!=(const Node& a, const Node& b) { return !(a.point == b.point); }
bool operator<(const Node& a, const Node& b) { return a.point < b.point; }
bool operator>(const Node& a, const Node& b) { return a.point > b.point; }

bool operator==(const AStarNode& a, const AStarNode& b) { return a.point == b.point; }
bool operator!=(const AStarNode& a, const AStarNode& b) { return !(a.point == b.point); }
bool operator<(const AStarNode& a, const AStarNode& b) { return a.point < b.point; }
bool operator>(const AStarNode& a, const AStarNode& b) { return a.point > b.point; }