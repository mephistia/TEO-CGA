#pragma once
#include <SFML/System/Vector2.hpp>
struct Node {
	int id;
	sf::Vector2<double> point; // Ponto do nodo (Site.p é igual)
	double cost = 0; // distância entre pontos
	Node* next; // próximo nodo
};
