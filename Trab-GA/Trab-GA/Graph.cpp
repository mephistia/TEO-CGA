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
	std::vector<Edge*> allEdges = diagram.edges;

	// ordenar por ponto com menor x (ponto à esquerda da edge)
	// possível caso: não existe um site
	std::sort(allEdges.begin(), allEdges.end(), [](Edge *a, Edge *b) {
		return a->lSite->p.x < b->lSite->p.x;
		});


	for (int i = 0; i < allEdges.size(); i++) {


		// só adiciona se existir os dois pontos
		// pois existem linhas que cortam com as bordas
		if (allEdges[i]->lSite &&
			allEdges[i]->rSite) {
			Node* newNode = new Node;
			newNode->edge = allEdges[i];

			// só existe próxima se não passar do tamanho
			if (i + 1 < allEdges.size()) {
				Node* _next = new Node;
				_next->edge = allEdges[i + 1];
			}
				

			// cálculo da distância dos pontos que a edge corta
			sf::Vertex begin(sf::Vector2f(allEdges[i]->lSite->p.x, allEdges[i]->lSite->p.y), sf::Color::Yellow);
			sf::Vertex end(sf::Vector2f(allEdges[i]->rSite->p.x, allEdges[i]->rSite->p.y), sf::Color::Yellow);
			double distance = dist(begin, end);
			newNode->cost = distance; // custo é a distância entre os pontos

			sf::Vector2<sf::Vertex> line(begin, end);
			sf::Vector2<sf::Vertex> lineInverse(end, begin);

			// criar line se ainda não existir (igual ou sentindo inverso)
			auto pos1 = std::find(lines.begin(), lines.end(), line);

			auto pos2 = std::find(lines.begin(), lines.end(), lineInverse);

			// se chegou no fim do contêiner nos 2 casos, é porque não existe
			if (pos1 == lines.end() && pos2 == lines.end()) {
				// adiciona
				lines.push_back(line);
				// imprimir a nova linha
				std::cout << "Linha: \n";
				std::cout << "(" << line.x.position.x << ", " << line.x.position.y << ") -> ";
				std::cout << "(" << line.y.position.x << ", " << line.y.position.y << ")\n";
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
}

