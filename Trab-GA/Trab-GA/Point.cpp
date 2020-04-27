#include "Point.h"


Point::Point()
{
	x = 0;
	y = 0;
	angle = -1;
	shape.setRadius(3.0);
	shape.setFillColor(sf::Color::White);
	shape.setPosition(x, y);
}

Point::~Point()
{
}

Point::Point(float a, float b)
{
	x = a;
	y = b;
	angle = -1;
	shape.setRadius(3.0);
	shape.setFillColor(sf::Color::White);
	shape.setPosition(x, y);
}

Point Point::operator=(const Point& P)
{
	x = P.x;
	y = P.y;
	return *this;
}

bool Point::operator==(const Point& P)
{
	return (x == P.x && y == P.y);
}

bool Point::operator<(const Point& P)
{
	return(angle < P.angle);
}


void Point::setX(float newX)
{
	x = newX;
}

float Point::getX()
{
	return x;
}

void Point::setY(float newY)
{
	y = newY;
}

float Point::getY()
{
	return y;
}

void Point::setAngle(double newAngle)
{
	angle = newAngle;
}

double Point::getAngle()
{
	return angle;
}

sf::CircleShape Point::getShape()
{
	return shape;
}
