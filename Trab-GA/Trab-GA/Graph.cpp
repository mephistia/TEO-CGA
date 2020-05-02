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
	nodeId = 0;

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
			Node* newNode = createNode(dest, distance, head[srcPos], nodeId);
			head[srcPos] = newNode;
			nodeId++;


			// Grafo de duas vias
			newNode = createNode(src, distance, head[destPos],nodeId);
			head[destPos] = newNode;
			nodeId++;
				
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
	BFS(head[0]->point);
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

Node* Graph::createNode(sf::Vector2<double> point, double cost, Node* head, int id)
{
	Node* newNode = new Node;
	newNode->point = point;
	newNode->cost = cost;
	newNode->next = head;
	newNode->id = id;
	return newNode;
}

// Recuperar ponteiro para a lista de destinos (direto)
Node* Graph::getHead(int id)
{
	return head[id];
}

// Recuperar ponteiro para a lista de destinos (por ponto)
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

// Retornar vetor dos pontos destino (melhor manipulação)
std::vector<sf::Vector2<double>> Graph::neighbors(sf::Vector2<double> point)
{
	Node* ptr = getHead(point);
	std::vector<sf::Vector2<double>> neighbors;
	while (ptr != nullptr) {
		neighbors.push_back(ptr->point);
		ptr = ptr->next;
	}
	return neighbors;
}

std::vector<Node*> Graph::neighborsN(Node* N)
{
	std::vector<Node*> neighbors;
	Node* ptr = getHead(N->point); // Não lançou o ponto?
	// N é head
	while (ptr != nullptr) {
		neighbors.push_back(ptr);
		ptr = ptr->next;
	}
	return neighbors;
}


// Breadth First Search
void Graph::BFS(sf::Vector2<double> startPoint)
{


	//// Mapear
	//std::queue<sf::Vector2<double>> frontier;
	//frontier.push(startPoint);

	//// Set de pontos únicos
	//std::unordered_set<sf::Vector2<double>> visited;
	//visited.insert(startPoint);

	// Tentar com Node
	Node* startPtr = getHead(startPoint);


	std::queue<Node*> frontierN;
	frontierN.push(startPtr);

	std::unordered_set<Node*> visitedN;
	visitedN.insert(startPtr);

	std::cout << "Visiting " << startPtr->point.x << ", " << startPtr->point.y << '\n';

	//while (!frontier.empty()) {
	//	sf::Vector2<double> current = frontier.front(); // copia primeiro elemento
	//	frontier.pop(); // remove primeiro elemento
	//	std::cout << "Visiting " << current.x << ", " << current.y << '\n';

	//	// Para cada destino
	//	for (sf::Vector2<double> next : neighbors(current)) {
	//		if (visited.find(next) == visited.end()) { // se não achou no visitado
	//			frontier.push(next); // passa como próxima fronteira
	//			visited.insert(next); // e insere como visitado
	//		}
	//	}
	//}

	while (!frontierN.empty()) {
		Node* current = frontierN.front(); // copia o primeiro elemento
		frontierN.pop();

		// Para cada destino
		for (Node* next : neighborsN(current)) {
			auto pos = std::find_if(visitedN.begin(), visitedN.end(), 
				[next](const Node* m) -> bool {return 
				((int)m->point.x == (int)next->point.x) && 
				((int)m->point.y == (int)next->point.y); 
				});
			std::cout << "Begin comparing nodes!" << "\n";
			if (pos == visitedN.end()) { // se não achou no visitado
				frontierN.push(next); // passa como próxima fronteira
				visitedN.insert(next); // e insere como visitado
				std::cout << "Visiting " << next->point.x << ", " << next->point.y << '\n';
			}
		}
	}
}




// Distância euclidiana
double Graph::dist(sf::Vertex p, sf::Vertex q)
{
	return std::sqrt(std::pow(q.position.x - p.position.x, 2) + std::pow(q.position.y - p.position.y, 2));
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

