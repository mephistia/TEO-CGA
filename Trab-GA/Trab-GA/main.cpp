#include "MyGAL/FortuneAlgorithm.h" // Voronoi
#include "Point.h"
#include <iostream>
#include <vector>
#include <stack>

// Angulo dos pontos
double getAngleWith(Point P, Point S)
{
    if (P.getX() == S.getX()) {
        return 0;
    }

    return (atan2(P.getY() - S.getY(), S.getX() - P.getX()) * (180.0 / M_PI));
}

// Pegar o segundo ponto na pilha
Point top2nd(std::stack<Point> & stack) {
    Point p = stack.top();
    stack.pop();
    Point second = stack.top();
    stack.push(p);
    return second;
}

// Anti-horário
bool ccw(Point p1, Point p2, Point p3) {
    return ((p2.getX() - p1.getX()) * (p3.getY() - p1.getY())
        - (p2.getY() - p1.getY()) * (p3.getX() - p1.getX())) <= 0;
}


std::vector<Point> grahamScan(std::vector<Point> points) {

    // Copia o vetor
    std::vector<Point> GS(points);

    std::cout << "Qtd de pontos: " << GS.size() << "\n";


    // Organizar os pontos pelo menor Y
    std::sort(GS.begin(), GS.end(), [](Point p1, Point p2) {
        return p1.getY() < p2.getY();
        });

    // Retorna o ponto com menor Y
    Point lsY = GS[0];

    // Remover duplicatas
    for (int i = 0; i < GS.size() - 1; i++) {
        if (GS[i] == GS[i + 1]) {
            GS.erase(GS.begin() + i);
            i--;
        }
    }

    // Se ficar < 3 pontos após remoção
    if (GS.size() < 3) {
        std::vector<Point> empty;
        return empty;
    }
    std::cout << "Nova qtd de pontos: " << GS.size() << "\n";

    // Ordena o restante por ângulo com eixo X
    for (int i = 1; i < GS.size(); i++) {
        double angle = getAngleWith(lsY,GS[i]);
        GS[i].setAngle(angle);
    }

    std::sort(GS.begin() + 1, GS.end(), [](Point p1, Point p2) {
        return p1.getAngle() < p2.getAngle();
        });
    // O primeiro ponto sempre ficará no início, pois tem ângulo -1 
    // e o menor possível do cálculo é 0

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
        while (!ccw(top2nd(hull), hull.top(), GS[i]) && hull.size() >= 3) {
            hull.pop();
        }
        hull.push(GS[i]);
    }

    std::vector<Point> res;

    while (!hull.empty()) {
        res.push_back(hull.top());
        hull.pop();
    }

    return res;
}




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
                    if (points.size() >= 3) {
                        instructions.setString("Generating Convex Hull (Graham Scan)...");

                        // Impedir o usuário de adicionar mais pontos
                        isUserDone = true;

                        
                        std::vector<Point> ch = grahamScan(points);

                        // Debug: Mostrar os pontos que formam o Hull
                        std::cout << std::endl << "Pontos p/ Convex Hull: " << ch.size() << std::endl;
                        if (ch.size() > 0) {
    
                            for (int i = 0; i < ch.size(); i++)
                            {
                                std::cout << "(" << ch[i].getX() << ", " << ch[i].getY() << ")" << std::endl;

                                
                                //// Criar linha com o próximo ponto
                                //if (i < ch.size() - 1) {
                                //    sf::Vertex v(sf::Vector2f(ch[i].getX(), ch[i].getY()), sf::Color::Blue, sf::Vector2f(ch[i + 1].getX(), ch[i + 1].getY()));
                                //}

                            }
                            // Flag para verificar se pode desenhar as linhas
                            isHullDone = true;                           

                            instructions.setString("Z = Toggle Hull. X = Toggle Graphs. C = Restart. Click = Select origin and waypoint.");

                        }
                        else {
                            instructions.setString("Insuficient points. Try unique points. C = Restart.");
                        }
                    }
                    else {
                        instructions.setString("At least 3 points required. Click = Add point. Spacebar = Compute stuff");
                    }
                }
            }


       
        }

        window.clear();

        // Desenhar os pontos
        for (Point& p : points) {
            window.draw(p.getShape());
        }
        window.draw(instructions);



        window.display();
    }

    return 0;
}