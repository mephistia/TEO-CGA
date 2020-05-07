#pragma once
#include <SFML/System/Vector2.hpp>
#include <unordered_set>
struct Node {
	Node(sf::Vector2<double> p) { point = p; }
	Node() {}
	~Node() {}
	sf::Vector2<double> point; // Ponto do nodo (Site.p é igual)
	std::vector<Node> neighbors; // vizinhos
};
