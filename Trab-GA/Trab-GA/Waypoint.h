#pragma once
#include <SFML/System/Vector2.hpp>

struct Waypoint {
	Waypoint(sf::Vector2<double> p) { point = p; }
	Waypoint(double x, double y) { point = sf::Vector2<double>(x,y); }
	sf::Vector2<double> point; // ponto "físico"
	std::set<Waypoint*> neighbors; // waypoints vizinhos (cada ponto só possui vizinhos únicos)
};
