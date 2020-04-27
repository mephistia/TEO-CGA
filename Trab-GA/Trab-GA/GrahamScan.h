#pragma once
#include "Point.h"
#include <iostream>
#include <vector>
#include <stack>
class GrahamScan
{
public:
	GrahamScan();
	~GrahamScan();

	std::vector<Point> CreateHull(std::vector<Point> points);

	bool PolarAngle(Point a, Point b);

	double sqrDist(Point a, Point b);

	int ccw(Point a, Point b, Point c);

	Point top2nd(std::stack<Point>& stack);

	double getAngleWith(Point P, Point S);


private:
	Point pivot;
};

