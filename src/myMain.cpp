#include <iostream>
#include <map>
#include "myMain.h"
#include "Drawing.h"
#include "colors.h"
#include <SFML/Graphics.hpp>

using namespace std;

int myMain()
{
    sf::RenderWindow window(sf::VideoMode(1280,720), "My Program");
    window.setFramerateLimit(60);

    sf::RectangleShape rect;
    sf::Vector2f rectanglePosition(0,350);
    rect.setPosition(rectanglePosition);
    rect.setSize(sf::Vector2f(100,100));

    //movement
    float xVelocity = 3;


    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed) window.close();

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
        }
        //physics
        rectanglePosition.x += xVelocity;
        rect.setPosition(rectanglePosition);

        //render
        window.clear();
        window.draw(rect);
        window.display();

    }


    return 0;
}