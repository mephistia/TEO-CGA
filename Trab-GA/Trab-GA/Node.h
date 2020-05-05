#pragma once
#include "Waypoint.h"
struct Node {
	Node(Waypoint *w, Node* n, double c) { waypoint = w; from = n; cost = c; }
	Waypoint* waypoint;
	double cost = 0; // dist�ncia entre pontos
	Node* from; // nodo pai
};
