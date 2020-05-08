#pragma once
#include "Node.h"
struct AStarNode
{
	AStarNode() {}
	~AStarNode() {}
	AStarNode(Node p, AStarNode* f) { point = p; from = f; }
	Node point;
	AStarNode* from;
	double f, gSoFar, h;
};

