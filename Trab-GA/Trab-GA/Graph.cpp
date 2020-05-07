#include "Graph.h"

Graph::Graph()
{
	AStarDone = false;
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
	for (int i = 0; i < allEdges.size(); i++) {

		// só adiciona se existir os dois pontos
		// pois existem linhas que cortam com as bordas
		if (allEdges[i]->lSite && allEdges[i]->rSite) {
			// cálculo da distância dos pontos que a edge corta
			sf::Vertex begin(sf::Vector2f(allEdges[i]->lSite->p), sf::Color::Yellow);
			sf::Vertex end(sf::Vector2f(allEdges[i]->rSite->p), sf::Color::Yellow);
							
			sf::Vector2<double> src = (sf::Vector2<double>)begin.position;
			sf::Vector2<double> dest = (sf::Vector2<double>)end.position;

			// Cria Edge (struct)
			mEdge newEdge;
			newEdge.src = src;
			newEdge.dest = dest;
			allmEdges.push_back(newEdge);

			// Edge ao contrário (2 vias)
			newEdge.src = dest;
			newEdge.dest = src;
			allmEdges.push_back(newEdge);

			// Se não existe, adiciona
			Node newNode(src);
			allPoints.insert(newNode);
			newNode.point = dest;
			allPoints.insert(newNode);


			// Linhas
			sf::Vector2<sf::Vertex> line(begin, end);

			// adiciona
			lines.push_back(line);

		}
	}
	buildNeighbors();
}

inline void Graph::buildNeighbors()
{
	Node src, dest;
	//int i = 0;
	for (mEdge edge : allmEdges) {
		// "getPoint"
		src = getHead(edge.src);
		dest = getHead(edge.dest);
		// pega esse ponto em allPoints
		// e coloca o dest como vizinho
		auto extract = allPoints.extract(src);
		src = extract.value();

		extract = allPoints.extract(dest);
		dest = extract.value();
		src.neighbors.push_back(dest);
		dest.neighbors.push_back(src);
		allPoints.insert(dest);
		allPoints.insert(src);
		//std::cout << "\r" << i << " de " << allmEdges.size() << std::flush;
		//i++;
	}
}

Node Graph::getHead(sf::Vector2<double> point)
{
	auto pos = std::find_if(allPoints.cbegin(), allPoints.cend(), [point](const Node& node) {
		return point == node.point;
		});

	if (pos != allPoints.cend()) {
		return *pos;
	}
	Node null;
	return null;
}


// A*
void Graph::AStar(sf::Vector2<double> startPoint, sf::Vector2<double> goalPoint)
{
	// Fila de pontos disponíveis (já organizados pelo menor custo)
	PriorityQueue open;

	// Colocar o primeiro
	AStarNode* start = new AStarNode(getHead(startPoint), nullptr, 0);
	open.put(start, start->cost);


	std::vector<Node> closed;

	//std::cout << "Visitando: " << startPoint.x << ", " << startPoint.y << '\n';


	while (!open.empty()) {

		AStarNode* current = open.get(); // copia o elemento

		//std::cout << "Ponto atual definido.\n";
		//Se chegou no ponto final, adiciona nos nodos e sai do loop
		if (current->point.point == goalPoint) {
			ANodes.push_back(current);
			//std::cout << "É o ponto destino! \n";
			break;
		}

		// Coloca o ponto na lista dos visitados
		closed.push_back(current->point);
		//std::cout << "Ponto está visitado.\n";

		 //Para cada destino
		for (Node neighbor : current->point.neighbors) {

			//std::cout << "Visitando um vizinho...\n";
			// Se não encontrou o ponto na lista de visitados
			if (std::find(closed.begin(), closed.end(), neighbor) == closed.end()) {
				//std::cout << "É um ponto novo!\n";
				// Calcular a heurística
				double heuristic = dist(neighbor.point, goalPoint);

				// Coloca na lista de pontos disponíveis
				AStarNode* newNode = new AStarNode(neighbor, current, heuristic);
				open.put(newNode, heuristic);
				//std::cout << "O ponto está disponível para visitação.\n\n";
			}
		}
	}
}

inline void Graph::buildAStarPath()
{
	//std::cout << "\nConstruindo caminho...\n";
	// primeiro nodo é o último que veio do cálculo
	if (ANodes.size() == 0) {
		std::cout << "\nNENHUM NODO ENCONTRADO!\n";
		return;
	}

	AStarNode* n = ANodes[ANodes.size() - 1];

	// enquanto não chegar no nodo inicial (sem nodo pai)
	while (n != nullptr) {
		//std::cout << "Colocando o ponto...\n";
		finalPath.push_back(n->point); // coloca o nodo do ponto final
		n = n->from; // e substitui pelo nodo pai
	}
	// inverte para ficar do início para o fim (pai para filhos)
	std::reverse(std::begin(finalPath), std::end(finalPath));
	//std::cout << "Todos os pontos adicionados e vetor invertido.\n\n";
	ANodes.clear();
}

void Graph::drawAStarPath(sf::Vector2<double> fromPoint, sf::Vector2<double> toPoint, sf::RenderWindow& window)
{
	if (!AStarDone) {
		AStar(fromPoint, toPoint);
		buildAStarPath();
		// gerar as linhas
		for (int i = 0; i < finalPath.size(); i++) {
			//std::cout << "Criando linhas do ponto " << i << ".\n";
			if (i != finalPath.size() - 1) {
				sf::Vertex begin(sf::Vector2f(finalPath[i].point), sf::Color::Green);
				sf::Vertex end(sf::Vector2f(finalPath[i + 1].point), sf::Color::Green);

				sf::Vector2<sf::Vertex> line(begin, end);

				path.push_back(line);
			}
		}
		allmEdges.clear();
		allPoints.clear();
		finalPath.clear();
		AStarDone = true;
	}

	
	//std::cout << "Desenhando linhas...\n";
	// desenhar as linhas
	for (sf::Vector2<sf::Vertex> vec : path) {

		sf::Vertex pathline[] = {
			vec.x,
			vec.y
		};

		window.draw(pathline, 2, sf::Lines);
	}
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
	ANodes.clear();
	finalPath.clear();
	path.clear();
	allmEdges.clear();
	allPoints.clear();
	AStarDone = false;
}

