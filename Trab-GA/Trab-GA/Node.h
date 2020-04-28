#pragma once
#include <SFML/System/Vector2.hpp>
struct Node {

	Edge* edge;
	double cost = 0; // distância
	Node* next; // próxima edge para cálculo
};
