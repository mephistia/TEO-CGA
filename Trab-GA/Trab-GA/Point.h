#define _USE_MATH_DEFINES
#pragma once
#include <math.h>   
#include "SFML/Graphics.hpp" // Círculo

class Point
{
public:
	Point();
	~Point();

	Point(float a, float b);

	Point operator = (const Point& P);

	bool operator == (const Point& P);

	bool operator < (const Point& P);

	void setX(float newX);
	float getX();

	void setY(float newY);
	float getY();

	void setAngle(double newAngle);
	double getAngle();
	
	sf::CircleShape getShape();


private:
	float x, y;
	double angle;
	// Definir Ponto
	sf::CircleShape shape;

};

