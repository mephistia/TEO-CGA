#pragma once
#include <SFML/System/Vector2.hpp>
struct Node {
	int id;
	sf::Vector2<double> point; // Ponto do nodo (Site.p � igual)
	double cost = 0; // dist�ncia entre pontos
	Node* next; // pr�ximo nodo
};
