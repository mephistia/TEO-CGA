#pragma once
#include <SFML/System/Vector2.hpp>
#include <vector>
struct Node {
	Node(sf::Vector2i p) { point = p; }
	Node() {}
	~Node() {}
	sf::Vector2i point; // Ponto do nodo (Site.p � igual)
	std::vector<Node> neighbors; // vizinhos
};
