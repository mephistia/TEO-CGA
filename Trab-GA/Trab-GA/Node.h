#pragma once
#include <SFML/System/Vector2.hpp>
struct Node {

	Edge* edge;
	double cost = 0; // dist�ncia
	Node* next; // pr�xima edge para c�lculo
};
