#pragma once
#include "Node.h"
struct AStarNode
{
	AStarNode(Node p, AStarNode* f, double c) { point = p; cost = c; from = f; }
	Node point;
	double cost;
	AStarNode* from;
};

