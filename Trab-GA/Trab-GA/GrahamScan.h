#pragma once
#include "Point.h"
#include <iostream>
#include <vector>
#include <stack>
#include <chrono>

#include <MyGAL/FortuneAlgorithm.h> // Voronoi


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

	template<typename T>
	void drawEdge(sf::RenderWindow& window, mygal::Vector2<T> origin, mygal::Vector2<T> destination, sf::Color color);

	template<typename T>
	void drawDiagram(sf::RenderWindow& window, const mygal::Diagram<T>& diagram);

    template<typename T>
    mygal::Diagram<T> generateDiagram(const std::vector<mygal::Vector2<T>>& points);

    template<typename T>
    std::vector<mygal::Vector2<T>> generatePoints(std::vector<Point> p);

    double normalize(double value, double min, double max);
    double denormalize(double value, double min, double max);

private:
	Point pivot;
    double Offset = 1.0f;
};

template<typename T>
inline void GrahamScan::drawEdge(sf::RenderWindow& window, mygal::Vector2<T> origin, mygal::Vector2<T> destination, sf::Color color)
{
	auto line = std::array<sf::Vertex, 2>
	{
		sf::Vertex(sf::Vector2f(origin.x, 1.0 - origin.y), color),
			sf::Vertex(sf::Vector2f(destination.x, 1.0 - destination.y), color)
	};
	window.draw(line.data(), 2, sf::Lines);
}

template<typename T>
inline void GrahamScan::drawDiagram(sf::RenderWindow& window, const mygal::Diagram<T>& diagram)
{
    for (const auto& site : diagram.getSites())
    {
        auto center = site.point;
        auto face = site.face;
        auto halfEdge = face->outerComponent;
        if (halfEdge == nullptr)
            continue;
        while (halfEdge->prev != nullptr)
        {
            halfEdge = halfEdge->prev;
            if (halfEdge == face->outerComponent)
                break;
        }
        auto start = halfEdge;
        while (halfEdge != nullptr)
        {
            if (halfEdge->origin != nullptr && halfEdge->destination != nullptr)
            {
                auto origin = (halfEdge->origin->point - center) * Offset + center;
                auto destination = (halfEdge->destination->point - center) * Offset + center;
                drawEdge(window, origin, destination, sf::Color::Red);
            }
            halfEdge = halfEdge->next;
            if (halfEdge == start)
                break;
        }
    }
}

template<typename T>
inline mygal::Diagram<T> GrahamScan::generateDiagram(const std::vector<mygal::Vector2<T>>& points)
{
    // Construct diagram
    auto algorithm = mygal::FortuneAlgorithm<T>(points);
    auto start = std::chrono::steady_clock::now();
    algorithm.construct();
    auto duration = std::chrono::steady_clock::now() - start;
    std::cout << "construction: " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "ms" << '\n';

    // Bound the diagram
    start = std::chrono::steady_clock::now();
    algorithm.bound(mygal::Box<T>{-0.05, -0.05, 1.05, 1.05}); // Take the bounding box slightly bigger than the intersection box
    duration = std::chrono::steady_clock::now() - start;
    std::cout << "bounding: " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "ms" << '\n';
    auto diagram = algorithm.getDiagram();

    // Intersect the diagram with a box
    start = std::chrono::steady_clock::now();
    diagram.intersect(mygal::Box<T>{0.0, 0.0, 1.0, 1.0});
    duration = std::chrono::steady_clock::now() - start;
    std::cout << "intersection: " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "ms" << '\n';

    return diagram;
}

template<typename T>
inline std::vector<mygal::Vector2<T>> GrahamScan::generatePoints(std::vector<Point> p)
{

    const int size = p.size();
    // Criar o vetor com Vector2
    std::vector<mygal::Vector2<T>> points[size];
    
    // Copiar normalizado (de 0-800 e 0-600 para 0-1)
    for (int i = 0; i < size; i++) {
        double n_x = normalize(p[i].getX(), 0.0, 1.0);
        double n_y = normalize(p[i].getY(), 0.0, 1.0);
        points[i] = mygal::Vector2<T>(n_x, n_y);
    }

    return points;

}
