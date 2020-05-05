#include "Graph.h"

Graph::Graph()
{
}

Graph::~Graph()
{
}

void Graph::generateGraph(Diagram diagram)
{
	// pegar edges
	allEdges = diagram.edges;
	allCells = diagram.cells;

	// ordenar por ponto com menor x (ponto à esquerda da edge)
	std::sort(allEdges.begin(), allEdges.end(), [](Edge *a, Edge *b) {
		return a->lSite->p.x < b->lSite->p.x;
		});

	std::sort(allCells.begin(), allCells.end(), [](Cell* a, Cell* b) {
		return a->site.p.x < b->site.p.x;
		});

	// Criar os nodos
	// Numero de pontos e numero de linhas
	int points = allCells.size(); // N
	int edges = allEdges.size(); // n
	std::cout << "Pontos: " << points << "\n";
	std::cout << "Edges: " << edges << "\n";

	for (int i = 0; i < edges; i++) {

		// só adiciona se existir os dois pontos
		// pois existem linhas que cortam com as bordas
		if (allEdges[i]->lSite && allEdges[i]->rSite) {
			std::cout << "Testando a Edge " << i << "\n";

			// Pega os pontos que a edge separa
			sf::Vertex begin(sf::Vector2f(allEdges[i]->lSite->p), sf::Color::Yellow);
			sf::Vertex end(sf::Vector2f(allEdges[i]->rSite->p), sf::Color::Yellow);

			// Cria os Waypoints com os pontos
			Waypoint* w1 = new Waypoint((int)begin.position.x, (int)begin.position.y);
			Waypoint *w2 = new Waypoint((int)end.position.x,(int)end.position.y);

			auto it1 = waypoints.find(*w1);
			auto it2 = waypoints.find(*w2);

			// se já existe esse ponto, extrai e insere com o novo vizinho
			if (it1 != waypoints.end()) {
				auto elem = waypoints.extract(it1);
				Waypoint w = elem.value();
				w.neighbors.insert(w2);
				waypoints.insert(w);
			}
			// se não, adiciona o vizinho e insere
			else {
				w1->neighbors.insert(w2);
				waypoints.insert(*w1);
			}

			if (it2 != waypoints.end()) {
				auto elem = waypoints.extract(it2);
				Waypoint w = elem.value();
				w.neighbors.insert(w1);
				waypoints.insert(w);
			}
			else {
				w2->neighbors.insert(w1);
				waypoints.insert(*w2);
			}

			// Linhas
			sf::Vector2<sf::Vertex> line(begin, end);

			// criar line
			auto pos1 = std::find(lines.begin(), lines.end(), line);

			// se chegou no fim do contêiner é porque não existe
			if (pos1 == lines.end()) {
				// adiciona
				lines.push_back(line);

				// Debug: imprimir a nova linha
				std::cout << "Line: ";
				std::cout << "(" << line.x.position.x << ", " << line.x.position.y << ") -> ";
				std::cout << "(" << line.y.position.x << ", " << line.y.position.y << ")\n";
			}
		}
	}

}


// Desenhar após o usuário clicar em dois pontos
void Graph::drawAStarPath(sf::Vector2<double> fromPoint, sf::Vector2<double> toPoint, sf::RenderWindow &window)
{
	Waypoint start((int)fromPoint.x, (int)fromPoint.y);
	Waypoint goal((int)toPoint.x, (int)toPoint.y);


	// retornar o iterador
	auto iS = waypoints.find(start);
	auto iG = waypoints.find(goal);

	if (iS == waypoints.end() || iG == waypoints.end())
		std::cout << "\nERROR: WAYPOINTS NOT FOUND\n";

	if (toPoint == fromPoint) {
		std::cout << "\nERROR: SAME WAYPOINTS\n";
		return;
	}
	// gerar o caminho com os pontos
	AStar(*iS, *iG);
	buildAStarPath();

	// gerar as linhas
	for (int i = 0; i < finalPath.size(); i++) {
		std::cout << "Creating lines from point " << i << ".\n";
	    if (i != finalPath.size() - 1) {
			sf::Vertex begin(sf::Vector2f(finalPath[i]->point), sf::Color::Green);
			sf::Vertex end(sf::Vector2f(finalPath[i+1]->point), sf::Color::Green);

			sf::Vector2<sf::Vertex> line(begin, end);

			path.push_back(line);
		}
	}
	std::cout << "Drawing lines...\n";
	// desenhar as linhas
	for (sf::Vector2<sf::Vertex> vec : path) {

		sf::Vertex pathline[] = {
			vec.x,
			vec.y
		};

		window.draw(pathline, 2, sf::Lines);
	}

}

void Graph::buildAStarPath()
{
	std::cout << "\nBuilding path...\n";
	// primeiro nodo é o último que veio do cálculo
	if (nodes.size() == 0) {
		std::cout << "\nNO NODES FOUND\n";
		return;
	}
		

	Node *n = nodes[nodes.size() - 1];

	// enquanto não chegar no nodo inicial (sem nodo pai)
	while (n != nullptr) {
		std::cout << "Pushing point...\n";
		finalPath.push_back(n->waypoint); // coloca o waypoint do ponto final
		n = n->from; // e substitui pelo nodo pai
	}
	// inverte para ficar do início para o fim (pai para filhos)
	std::reverse(std::begin(finalPath), std::end(finalPath));
	std::cout << "All points pushed.\n\n";
}

// A*
void Graph::AStar(Waypoint startPoint, Waypoint goalPoint)
{
	if (startPoint == goalPoint) {
		std::cout << "\nERROR: SAME POINT\n";
		return;
	}

	// Lista de pontos disponíveis (já organizados pelo menor custo)
	PriorityQueue open;

	// Criar nodo para o início e colocar como ponto disponível
	Node* start = new Node(&startPoint, nullptr, 0);
	open.put(start, start->cost);

	// Lista de pontos visitados 
	std::vector<Waypoint> closed;

	std::cout << "Visiting " << startPoint.point.x << ", " << startPoint.point.y << '\n';

	// Enquanto ainda houver nodos disponíveis para visitar
	while (!open.empty()) {

		Node* current = open.get(); // coloca o nodo e remove do open
		std::cout << "Point current set.\n";
		 //Se chegou no ponto final, adiciona nos nodos e sai do loop
		if (current->waypoint == &goalPoint) {
			nodes.push_back(current);
			std::cout << "Goal point! \n";
			break;
		}

		// Teste
		if (current == nullptr) {
			break;
		}

		// Coloca o ponto na lista dos visitados
		closed.push_back(*current->waypoint);
		std::cout << "Point is now closed.\n";


		 // Para cada vizinho do atual
		for (Waypoint* neighbor : current->waypoint->neighbors) {
			std::cout << "Visiting a neighbor.\n";
			// Se não encontrou o ponto na lista de visitados
			if (std::find(closed.begin(), closed.end(), *neighbor) == closed.end()) {
				std::cout << "It's a new point!.\n";
				// Calcular a heurística
				double heuristic = calcHeuristic(*neighbor, goalPoint);

				// Coloca na lista de pontos disponíveis
				Node* newNode = new Node(neighbor, current, heuristic);
				open.put(newNode,heuristic);
				std::cout << "Point is now open.\n\n";
			}
		}
	}
}

inline double Graph::calcHeuristic(Waypoint w1, Waypoint w2)
{
	return dist(w1.point,w1.point);
}

// Distância euclidiana
double Graph::dist(sf::Vertex p, sf::Vertex q)
{
	return std::sqrt(std::pow(q.position.x - p.position.x, 2) + std::pow(q.position.y - p.position.y, 2));
}

double Graph::dist(sf::Vector2<double> p, sf::Vector2<double> q)
{
	return std::sqrt(std::pow(q.x - p.x, 2) + std::pow(q.y - p.y, 2));
}


void Graph::drawGraph(sf::RenderWindow &window)
{
	// desenhar as linhas
	for (sf::Vector2<sf::Vertex> vec : lines) {

		sf::Vertex line[] = {
			vec.x,
			vec.y
		};

		window.draw(line, 2, sf::Lines);
	}
}

void Graph::clearGraph()
{
	lines.clear();
}

void Graph::clearAStarPath()
{
	waypoints.clear();
	finalPath.clear();
	nodes.clear();
	path.clear();
}

