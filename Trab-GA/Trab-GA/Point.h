#define _USE_MATH_DEFINES
#pragma once
constexpr auto w_width = 900;
constexpr auto w_height = 900;

#include <math.h>   
#include "SFML/Graphics.hpp" // C�rculo
#include <VoronoiDiagramGenerator.h>

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

	sf::Vector2<double> getVector();

private:
	float x, y;
	double angle;
	// Definir Ponto
	sf::CircleShape shape;

};

