#include "GrahamScan.h"



int main()
{
    // Criar janela
    sf::RenderWindow window(sf::VideoMode(800, 600), "Melhor Rota");

    // Criar vetor de pontos
    std::vector<Point> points;

    // Flags
    bool isUserDone = false;
    bool isHullDone = false;
    bool isWaypointChosen = false;

    // Texto que aparece na tela
    sf::Text instructions;

    // Algoritmo
    GrahamScan gScan;

    // Vetor de vértices
    sf::VertexArray convexHull(sf::LineStrip);

    // Pontos no Convex Hull
    std::vector<Point> ch;
   


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
        instructions.setPosition(10, 570);
    }


    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Identificar clique
            else  if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    // Pegar posição do mouse
                    sf::Vector2i mPos = sf::Mouse::getPosition(window);

                    // Criar Ponto se estiver dentro da margem
                    if ((mPos.x > 25 && mPos.x < 775) &&
                        (mPos.y > 25 && mPos.y < 575) &&
                        isUserDone == false) {
                        Point mousePoint(mPos.x, mPos.y);
                        points.push_back(mousePoint);
                    }


                }
            }

            // Identificar barra de espaço
            else if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Space){

                    // Conferir quantidade de pontos após remover duplicatas
                    if (points.size() >= 3 && !isUserDone) {
                        instructions.setString("Generating Convex Hull (Graham Scan)...");

                        // Impedir o usuário de adicionar mais pontos
                        isUserDone = true;

                        
                        ch = gScan.CreateHull(points);

                        // Debug: Mostrar os pontos que formam o Hull
                        std::cout << std::endl << "Pontos p/ Convex Hull: " << ch.size() << std::endl;
                        if (ch.size() > 0) {
    
                            for (int i = 0; i < ch.size(); i++)
                            {
                                std::cout << "(" << ch[i].getX() << ", " << ch[i].getY() << ")" << std::endl;

                                // Adicionar ponto ao array
                                sf::Vertex v(sf::Vector2f(ch[i].getX(), ch[i].getY()), sf::Color::Blue);
                                convexHull.append(v);

                            }
                            // Adicionar o último ponto para fechar
                            convexHull.append(convexHull[0]);

                            // Flag para verificar se pode desenhar as linhas
                            isHullDone = true;                           

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
                    if (isUserDone) {
                        points.clear();
                        ch.clear();

                        if (isHullDone) {
                            convexHull.clear();
                            isHullDone = false;
                        }
                        instructions.setString("Click = Add point. Spacebar = Compute stuff");
                        isUserDone = false;
                    }

                }
            }


       
        }

        // Calcular Voronoi (e desenhar grafos)
        auto voronoi = gScan.generateDiagram(gScan.generatePoints<Point>(ch));
        // mudar para pontos normalizados
        window.setView(sf::View(sf::FloatRect(-0.1f, -0.1f, 1.2f, 1.2f)));

        window.clear();

        // Desenhar os pontos
        for (Point& p : points) {
            window.draw(p.getShape());
        }
        window.draw(instructions);

        if (isHullDone) {
            window.draw(convexHull);
            // voronoi
            gScan.drawDiagram(window, voronoi);
        }

        

        window.display();
    }

    return 0;
}