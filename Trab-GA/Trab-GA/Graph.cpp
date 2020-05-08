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
	// ordenar por ponto com menor x
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
							
			sf::Vector2i src = sf::Vector2i((int)begin.position.x, (int)begin.position.y);
			sf::Vector2i dest = sf::Vector2i((int)end.position.x, (int)end.position.y);

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
			auto extract = allPoints.extract(newNode);
			if (extract) {
				newNode = extract.value();
			}
			Node newNode2(dest);
			extract = allPoints.extract(newNode2);
			if (extract) {
				newNode2 = extract.value();
			}
			newNode.neighbors.push_back(newNode2);
			newNode2.neighbors.push_back(newNode);
			allPoints.insert(newNode);
			allPoints.insert(newNode2);


			// Linhas
			sf::Vector2<sf::Vertex> line(begin, end);

			// adiciona
			lines.push_back(line);

		}
	}
}

Node Graph::getHead(sf::Vector2i point)
{
	auto pos = std::find_if(allPoints.cbegin(), allPoints.cend(), [&point](const Node& node) {
		return point == node.point;
		});

	if (pos != allPoints.cend()) {
		return *pos;
	}
	Node null;
	return null;
}


// A*
void Graph::AStar(sf::Vector2i startPoint, sf::Vector2i goalPoint)
{

	// Colocar o primeiro
	AStarNode* start = new AStarNode(getHead(startPoint), nullptr);
	start->gSoFar = 0;
	pq.insert(std::make_pair(0,start));
	start->f = start->gSoFar + dist(startPoint, goalPoint);

	std::vector<Node> closed; // Pontos visitados
	

	while (!pq.empty()) {

		PQelem element = *pq.begin();
		AStarNode* current = element.second; // pega o nodo

		//Se chegou no ponto final, adiciona nos nodos e sai do loop
		if (current->point.point == goalPoint) {
			ANodes.push_back(current);
			break;
		}

		// Coloca o ponto na lista dos visitados
		pq.erase(element);
		closed.push_back(current->point);
		std::cout << "Testando ponto " << current->point.point.x << ", " << current->point.point.y << "\n";
		
		//Para cada destino do atual
		for (AStarNode* neighbor : getNeighbors(current)) {
			// Se não encontrou o ponto na lista de visitados
			Node point = neighbor->point;
			auto pos = std::find_if(closed.cbegin(), closed.cend(), [&point](const Node& n) {
				return point.point == n.point;
				});
			if (pos == closed.end()) {
				// calcula heuristica
				neighbor->f = neighbor->gSoFar + dist(neighbor->point.point, goalPoint);
				// verifica se existe nos pontos disponíveis
				auto pos2 = std::find_if(pq.begin(), pq.end(), [neighbor](PQelem a) {
					return a.second == neighbor;
					});
				// se nao existe, insere
				if (pos2 == pq.end()) {
					pq.insert(std::make_pair(neighbor->f, neighbor));
				}
				// se existe
				else {
					element = *pos2;
					AStarNode* openNeighbor = element.second;
					// e se tiver custo menor, adiciona
					if (neighbor->gSoFar < openNeighbor->gSoFar) {
						openNeighbor->gSoFar = neighbor->gSoFar;
						openNeighbor->from = neighbor->from;
					}
				}

			}
		}
	}
}

inline std::vector<AStarNode*> Graph::getNeighbors(AStarNode* n)
{
	std::vector<AStarNode*> nodes;
	for (Node neighbor : n->point.neighbors) {
		AStarNode* an = new AStarNode;
		an->point = neighbor;
		an->gSoFar = n->gSoFar + 1;
		an->from = n;
		nodes.push_back(an);
	}

	return nodes;

}

inline void Graph::buildAStarPath()
{
	// primeiro nodo é o último que veio do cálculo
	if (ANodes.size() == 0) {
		std::cout << "\nNENHUM NODO ENCONTRADO!\n";
		return;
	}

	AStarNode* n = ANodes.back();
	// enquanto não chegar no nodo inicial (sem nodo pai)
	while (n != nullptr) {
		finalPath.push_back(n->point); // coloca o nodo do ponto final
		n = n->from; // e substitui pelo nodo pai
	}

	// inverte para ficar do início para o fim 
	std::reverse(std::begin(finalPath), std::end(finalPath));
}

void Graph::drawAStarPath(sf::Vector2i fromPoint, sf::Vector2i toPoint, sf::RenderWindow& window)
{
 	if (!AStarDone) {
		AStar(fromPoint, toPoint);
		buildAStarPath();
		// gerar as linhas
		for (int i = finalPath.size() - 1; i > 0; i--) {

			sf::Vertex begin(sf::Vector2f(finalPath[i].point), sf::Color::Green);
			sf::Vertex end(sf::Vector2f(finalPath[i - 1].point), sf::Color::Green);

			sf::Vector2<sf::Vertex> line(begin, end);

			path.push_back(line);
			
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

double Graph::dist(sf::Vector2i p, sf::Vector2i q)
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

