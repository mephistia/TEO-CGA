#define _USE_MATH_DEFINES
#pragma once
constexpr auto w_width = 900;
constexpr auto w_height = 900;

#include <math.h>   
#include "SFML/Graphics.hpp" // Círculo
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

	void setX(int newX);
	float getX();

	void setY(int newY);
	float getY();

	void setAngle(double newAngle);
	double getAngle();
	
	sf::CircleShape getShape();

	void setColor(sf::Color c);

	sf::Vector2<double> getVector();

private:
	int x, y;
	double angle;
	// Definir Ponto
	sf::CircleShape shape;

};

