/*
Biblioteca multim�dia SFML (Simple and Fast Multimedia Library)
    https://www.sfml-dev.org/
Biblioteca de Voronoi por Mark Dally (adaptada por Jonny Paton para SFML)
    https://github.com/JonnyPtn/Voronoi
*/

#include "Graph.h"

void genSites(std::vector<sf::Vector2<double>>& sites, sf::Rect<double>& bbox, unsigned int numSites, std::vector<Point> points) {
    sites.reserve(numSites);

    for (Point& p : points) {
        sites.push_back(p.getVector());
    }

}

int main()
{
    // Criar janela
    sf::RenderWindow window(sf::VideoMode(w_width, w_height), "Melhor Rota");

    // Criar vetor de pontos
    std::vector<Point> points;

    // Flags
    bool isUserDone = false;
    bool isHullDone = false;
    bool isVoronoiDone = false;
    bool isWaypointChosen = false;

    bool toggleHull = true;
    bool toggleVoronoi = true;
    bool toggleGraphs = true;

    // Texto que aparece na tela
    sf::Text instructions;

    // Graham Scan
    GrahamScan gScan;

    // Vetor de v�rtices
    sf::VertexArray convexHull(sf::LineStrip);

    // Pontos no Convex Hull
    std::vector<Point> ch;

    // Gerar Voronoi
    std::vector<sf::Vector2<double>>* sites;
    VoronoiDiagramGenerator vdg = VoronoiDiagramGenerator();
    std::unique_ptr<Diagram> diagram;
    std::vector<sf::Vertex> vertices;

    // Gerar Grafo
    Graph graph;

    // Pontos para A*
    sf::Vector2i start = sf::Vector2i(0,0), goal = sf::Vector2i(0, 0);

    // Gerar desenho
    auto updateVisuals = [&]() {
        vertices.clear();
        vertices.reserve(diagram->cells.size() + (diagram->edges.size() * 2));
        for (auto c : diagram->cells)
        {
            sf::Vector2<double>& p = c->site.p;
            vertices.push_back({ { static_cast<float>(p.x),static_cast<float>(p.y)}, sf::Color::White });
        }

        for (Edge* e : diagram->edges)
        {
            if (e->vertA && e->vertB)
            {
                sf::Vector2<double>& p1 = *e->vertA;
                sf::Vector2<double>& p2 = *e->vertB;

                vertices.push_back({ { static_cast<float>(p1.x),static_cast<float>(p1.y) },sf::Color(50,50,50) });
                vertices.push_back({ { static_cast<float>(p2.x),static_cast<float>(p2.y) },sf::Color(50,50,50) });
            }
        }
    };


    // Gerar Voronoi
    auto generateVoronoi = [&](int nPoints) {
        sites = new std::vector<sf::Vector2<double>>();
        sf::Rect<double> bbox = gScan.getBBox();
        genSites(*sites, bbox, nPoints, points);
        Diagram* compute = vdg.compute(*sites, bbox);
        graph.generateGraph(*compute);
        diagram.reset(compute);
        delete sites;
        isVoronoiDone = true;
    };


    sf::Font font;
    if (!font.loadFromFile("Dosis-Medium.ttf"))
    {
        std::cout << "Fonte nao encontrada!" << std::endl;
    }
    else {
        instructions.setFont(font);
        instructions.setString("Click = Add point. Spacebar = Compute stuff");
        instructions.setCharacterSize(18);
        instructions.setFillColor(sf::Color::White);
        instructions.setPosition(10, w_height - 35);
    }


    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type){
            
            case sf::Event::Closed:
                    window.close();

                // Identificar clique
            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == sf::Mouse::Left)
                { 
                    // Pegar posi��o do mouse
                    sf::Vector2f mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                    // Criar Ponto se estiver dentro da margem e
                    // o usu�rio ainda n�o tiver apertado espa�o
                    if ((mPos.x > 25 && mPos.x < w_width - 25) &&
                        (mPos.y > 25 && mPos.y < w_height - 25) &&
                        !isUserDone) {
                        Point mousePoint(mPos.x, mPos.y);
                        points.push_back(mousePoint);
                    }

                    // se estiver na �rea de um ponto (3 pixels de diferen�a
                    else if (isUserDone) {
                        if (start == sf::Vector2i(0, 0)) {
                            for (Point& p : points) {
                                if (p.getShape().getGlobalBounds().contains(mPos)) {
                                    // deixar verde
                                    p.setColor(sf::Color(0, 255, 174));
                                    start.x = (int)p.getX();
                                    start.y = (int)p.getY();
                                }
                            }
                        }
                        else if (goal == sf::Vector2i(0, 0)) {
                            for (Point& p : points) {
                                if (p.getShape().getGlobalBounds().contains(mPos)) {
                                    // deixar verde
                                    p.setColor(sf::Color(0, 255, 174));
                                    goal.x = p.getX();
                                    goal.y = p.getY();
                                    isWaypointChosen = true;

                                }
                            }
                        }
                        
                    }
                }
                

                // Identificar barra de espa�o
            case sf::Event::KeyReleased:
                if (event.key.code == sf::Keyboard::Space){

                    // Conferir quantidade de pontos ap�s remover duplicatas
                    if (points.size() >= 3 && !isUserDone) {
                        instructions.setString("Generating Convex Hull (Graham Scan)...");

                        // Impedir o usu�rio de adicionar mais pontos
                        isUserDone = true;

                        ch = gScan.CreateHull(points);

                        if (ch.size() > 0) {
    
                            for (int i = 0; i < ch.size(); i++)
                            {                                
                                // Adicionar ponto ao array
                                sf::Vertex v(sf::Vector2f(ch[i].getX(), ch[i].getY()), sf::Color::Blue);
                                convexHull.append(v);
                            }
                            // Adicionar o �ltimo ponto para fechar
                            convexHull.append(convexHull[0]);

                            // Flag para verificar se pode desenhar as linhas
                            isHullDone = true;                           
                            toggleGraphs = true;
                            toggleHull = true;
                            instructions.setString("Z = Toggle Hull. X = Toggle Graphs. C = Restart. Click = Select origin and waypoint.");
                        }
                        else {
                            instructions.setString("Insuficient points. Try unique points. C = Restart.");
                        }
                    }
                    else if (points.size() < 3){
                        instructions.setString("At least 3 points required. Click = Add point. Spacebar = Compute stuff");
                    }
                }

                // Resetar
                else if (event.key.code == sf::Keyboard::C) {
                    if (isUserDone && isHullDone) {
                        convexHull.clear();
                        graph.clearGraph();
                        graph.clearAStarPath();
                        isHullDone = false;
                        isVoronoiDone = false;
                        points.clear();
                        ch.clear();
                        instructions.setString("Click = Add point. Spacebar = Compute stuff");
                        isUserDone = false;
                        isWaypointChosen = false;
                        start = sf::Vector2i(0, 0);
                        goal = sf::Vector2i(0, 0);
                    }

                }
                else if (event.key.code == sf::Keyboard::X) {
                    if (isHullDone) {
                        toggleGraphs = !toggleGraphs;
                    }
                }
                else if (event.key.code == sf::Keyboard::Z) {
                    if (isHullDone)
                        toggleHull = !toggleHull;
                }
            }

        }
       
        window.clear();

        // Desenhar os pontos
        for (Point& p : points) {
            window.draw(p.getShape());
        }
        window.draw(instructions);

        if (isHullDone) {
            // Desenhar Voronoi
            auto pointCount = points.size();

            if (!isVoronoiDone)
                generateVoronoi(pointCount);

            updateVisuals();
            window.draw(vertices.data(), pointCount, sf::PrimitiveType::Points);
            window.draw(vertices.data() + pointCount, vertices.size() - pointCount, sf::PrimitiveType::Lines);

            if (toggleGraphs) {
                // Desenhar grafos
                graph.drawGraph(window);
            }


            // Desenhar Convex Hull
            if (toggleHull)
                window.draw(convexHull);

            // desenhar caminho
            if (isWaypointChosen) {
                sf::Vector2i P1 = sf::Vector2i(start.x, start.y);
                sf::Vector2i P2 = sf::Vector2i(goal.x, goal.y);

                graph.drawAStarPath(P1, P2, window);
            }
            
        }
        window.display();
    }

    return 0;
}


