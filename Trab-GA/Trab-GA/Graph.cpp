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
	
	head = new Node * [points];
	for (int i = 0; i < points; i++) {
		head[i] = nullptr;
	}


	for (int i = 0; i < edges; i++) {

		// só adiciona se existir os dois pontos
		// pois existem linhas que cortam com as bordas
		if (allEdges[i]->lSite && allEdges[i]->rSite) {
			std::cout << "Testando a Edge " << i << "\n";
			// cálculo da distância dos pontos que a edge corta
			sf::Vertex begin(sf::Vector2f(allEdges[i]->lSite->p), sf::Color::Yellow);
			sf::Vertex end(sf::Vector2f(allEdges[i]->rSite->p), sf::Color::Yellow);
			
			double distance = dist(begin, end);
				
			sf::Vector2<double> src = (sf::Vector2<double>)begin.position;
			sf::Vector2<double> dest = (sf::Vector2<double>)end.position;

			int srcPos = -1;
			int destPos = -1;

			// Encontrar a posição de src e dest
			for (int i = 0; i < points; i++) {
				if ((int)src.x == (int)allCells[i]->site.p.x &&
					(int)src.y == (int)allCells[i]->site.p.y) {
					srcPos = i;
				}
				if ((int)dest.x == (int)allCells[i]->site.p.x &&
					(int)dest.y == (int)allCells[i]->site.p.y) {
					destPos = i;
				}
			}

			// Nodo de adjacência
			Node* newNode = createNode(dest, distance, head[srcPos]);
			head[srcPos] = newNode;

			// Grafo de duas vias
			newNode = createNode(src, distance, head[destPos]);
			head[destPos] = newNode;
			
			// Linhas
			sf::Vector2<sf::Vertex> line(begin, end);
			sf::Vector2<sf::Vertex> lineEnd(end, begin);

			// criar line se ainda não existir
			auto pos1 = std::find(lines.begin(), lines.end(), line);

			// se chegou no fim do contêiner é porque não existe
			if (pos1 == lines.end()) {
				// adiciona
				lines.push_back(line);
				// imprimir a nova linha
				std::cout << "Line: ";
				std::cout << "(" << line.x.position.x << ", " << line.x.position.y << ") -> ";
				std::cout << "(" << line.y.position.x << ", " << line.y.position.y << ")\n";
			}
		}
	}


	// teste para imprimir os nodos
	for (int i = 0; i < points; i++) {
		std::cout << "Point " << i << ": \n";
		// Cada ponto tem seu conjunto de pontos
	
		// site.p (ponto source)
		// head[i] (ponteiro para o início da lista de destinos do source)
		printNodes(head[i], allCells[i]->site.p);
	}

	// teste de visitação
	//BFS(head[0]->point, head[points-1]->point);
	AStar(head[0]->point, head[points - 1]->point);
}

void Graph::printNodes(Node* ptr, sf::Vector2<double> i)
{
	while (ptr != nullptr)
	{
		std::cout << "(" << "{" << i.x << " ," << i.y << "}" << ", " << "{" << ptr->point.x << " ," << ptr->point.y << "}"
			<< ", Cost: " << ptr->cost << ") \n";

		ptr = ptr->next;
	}

	std::cout << "\n";
}

Node* Graph::createNode(sf::Vector2<double> point, double cost, Node* head)
{
	Node* newNode = new Node;
	newNode->point = point;
	newNode->cost = cost;
	newNode->next = head;
	return newNode;
}


// Recuperar ponteiro para a lista de destinos do ponto
Node* Graph::getHead(sf::Vector2<double> point)
{
	// tamanho de head é allCells.size()
	for (int i = 0; i < allCells.size(); i++) {

		// Converter para int para evitar desvio imperceptível (0.00006)
		if ((int)point.x == (int)head[i]->point.x &&
			(int)point.y == (int)head[i]->point.y) {
			return head[i];
		}
	}
	return nullptr;
}

// Encontrar nodos de destino
std::vector<Node*> Graph::neighborsN(Node* N)
{
	std::vector<Node*> neighbors;
	Node* ptr = getHead(N->point);

	while (ptr != nullptr) {
		neighbors.push_back(ptr);
		ptr = ptr->next;
	}
	return neighbors;
}

std::vector<Node*> Graph::neighborsN(sf::Vector2<double> N)
{
	std::vector<Node*> neighbors;

	Node* ptr = getHead(N);

	while (ptr != nullptr) {
		neighbors.push_back(ptr);
		ptr = ptr->next;
	}

	return neighbors;
}


// A*
void Graph::AStar(sf::Vector2<double> startPoint, sf::Vector2<double> goalPoint)
{
	PriorityQueue frontier;
	frontier.put(startPoint, 0);

	std::unordered_map<sf::Vector2<double>, sf::Vector2<double>> cameFrom;
	cameFrom[startPoint] = startPoint;

	std::unordered_map<sf::Vector2<double>, double> distance;
	distance[startPoint] = 0;

	std::cout << "Visiting " << startPoint.x << ", " << startPoint.y << '\n';


	while (!frontier.empty()) {
		sf::Vector2<double> current = frontier.get(); // copia o elemento

		 //Se chegou no ponto final, sai do loop
		if (current == goalPoint)
			break;

		 //Para cada destino
		 //~com vector para evitar for dentro de for O(N²)~
		for (Node* next : neighborsN(current)) {
			 //Calcula novo custo
			double newCost = distance[current] + dist(current, next->point);

			auto pos = std::find_if(distance.begin(), distance.end(), 
				[next](std::pair<const sf::Vector2<double>, double> n) -> bool {return
				((int)n.first.x == (int)next->point.x) && 
				((int)n.first.y == (int)next->point.y); 
				});

			 //se o custo do próximo é = ou < que a distância
			if (distance.find(next->point) == distance.end() || newCost < distance[next->point]) { 
				// define nova distância
				distance[next->point] = newCost;
				// adiciona na fronteira com prioridade do custo atual
				// até o calculo do custo final
				double priority = newCost + dist(next->point, goalPoint);
				frontier.put(next->point, priority);
				cameFrom[next->point] = current;
				std::cout << "Visiting " << next->point.x << ", " << next->point.y << '\n';
			}
		}
	}

	std::cout << "Goal point! \n";

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
	head = nullptr;
}

