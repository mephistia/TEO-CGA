#include "GrahamScan.h"

GrahamScan::GrahamScan()
{
}

GrahamScan::~GrahamScan()
{
}

std::vector<Point> GrahamScan::CreateHull(std::vector<Point> points)
{
    // Vetor p/ resultado
    std::vector<Point> res;

    // Copia o vetor de pontos
    std::vector<Point> GS(points);

    std::cout << "Qtd de pontos: " << GS.size() << "\n";


    // Organizar os pontos pelo menor Y (sobrecarga de operador)
    std::sort(GS.begin(), GS.end(), [](Point p1, Point p2) {
        return p1 < p2;
        });

    // Retorna o ponto com menor Y
    pivot = GS[0];

    // Remover duplicatas
    for (int i = 0; i < GS.size() - 1; i++) {
        if (GS[i] == GS[i + 1]) {
            GS.erase(GS.begin() + i);
            i--;
        }
    }

    // Se ficar < 3 pontos após remoção
    if (GS.size() < 3) {
        return res; // retorna o resultado vazio
    }
    std::cout << "Nova qtd de pontos: " << GS.size() << "\n";

    // Ordena o restante por ângulo com eixo X
    std::sort(GS.begin() + 1, GS.end(), [this](Point a, Point b) {
        return PolarAngle(a, b);
    });


    // Criar pilha de pontos para o Convex Hull
    // os dois primeiros pontos sempre entram
    // o terceiro já entra para teste
    std::stack<Point> hull;
    hull.push(GS[0]);
    hull.push(GS[1]);
    hull.push(GS[2]);

    // Remover da pilha enquanto [i] não fizer ângulo CCW com os anteriores
    // depois incluir no Hull
    for (int i = 3; i < GS.size(); i++) {
        Point top = hull.top();
        hull.pop();
        while (ccw(hull.top(), top, GS[i]) != -1) {
            top = hull.top();
            hull.pop();
        }
        hull.push(top);
        hull.push(GS[i]);
    }


    // Converte para vetor
    while (!hull.empty()) {
        res.push_back(hull.top());
        hull.pop();
    }

    return res;
}

// Verifica se entra no Hull (possui ângulo ccw)
bool GrahamScan::PolarAngle(Point a, Point b)
{
	int order = ccw(pivot, a, b);
	if (order == 0)
		return sqrDist(pivot, a) < sqrDist(pivot, b);
	return (order == -1);
}

// Distância Euclidiana ao quadrado
double GrahamScan::sqrDist(Point a, Point b)
{
	double dx = a.getX() - b.getX(), dy = a.getY() - b.getY();
	return dx * dx + dy * dy;
}


// Anti-horário
int GrahamScan::ccw(Point a, Point b, Point c)
{
	int res = (b.getX() - a.getX()) * (c.getY() - a.getY()) - (b.getY() - a.getY()) * (c.getX() - a.getX());
	if (res > 0)
		return -1;
	else if (res < 0)
		return 1;
	return 0;
}

// Pegar o segundo ponto na pilha
Point GrahamScan::top2nd(std::stack<Point>& stack)
{
	Point p = stack.top();
	stack.pop();
	Point second = stack.top();
	stack.push(p);
	return second;
}

// Ângulo entre dois pontos e o eixo x
double GrahamScan::getAngleWith(Point P, Point S)
{
	if (P.getX() == S.getX()) {
		return 0;
	}

	return (atan2(P.getY() - S.getY(), S.getX() - P.getX()) * (180.0 / M_PI));
}

double GrahamScan::normalize(double value, double min, double max)
{
    return (value - min) / (max - min);
}

double GrahamScan::denormalize(double value, double min, double max)
{
    return (value * (max - min) + min);
}
