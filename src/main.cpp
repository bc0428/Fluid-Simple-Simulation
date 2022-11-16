//
// Created by Brian Cheng on 14/11/2022.
//

#include "FluidCube2D.h"
#include "iostream"
#include "math.h"
#include "SFML/Graphics.hpp"
int w = SIZE;
int h = SIZE;

int main()
{
//    mode, window title name, style(what functions to include in title bar)
    sf::RenderWindow window(sf::VideoMode(SIZE,SIZE), "Window", sf::Style::Close | sf::Style::Titlebar | sf::Style::Resize);
    window.setFramerateLimit(1);

    FluidCube cube(0.1,0,0);

    sf::Texture texture;
    texture.create(w,h);

    sf::Sprite sprite(texture);
    sf::Uint8* pixels = new sf::Uint8 [h * w * 4];



// create loop for window to stay open
    while (window.isOpen()){

        sf::Event evnt;


        for (int i=0; i< h; i++){
            for (int j=0; j<w; j++){

                pixels[(i*w + j)*4]    = (sf::Uint8)cube.density[i*w + j];
                pixels[(i*w + j)*4 +1] = (sf::Uint8)cube.density[i*w + j];
                pixels[(i*w + j)*4 +2] = (sf::Uint8)cube.density[i*w + j];
                pixels[(i*w + j)*4 +3] = (sf::Uint8)255;
            }
        }
        for (int i=0; i<h; i++){
            for (int j=0; j<w; j++){
                std::cout << cube.density[i*w + j] << ' ';
            }
            std::cout << '\n';
        }

        float prev_x = 0;
        float prev_y = 0;
//        window activities
        while (window.pollEvent(evnt)){
//        close window case
            switch(evnt.type){
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseButtonPressed:
                    sf::Vector2i pos = sf::Mouse::getPosition(window);
                    int x = static_cast<float>(pos.x);
                    int y = static_cast<float>(pos.y);

                    cube.FluidCubeAddDensity(x,y, 100);
                    float VelX =  x - prev_x;
                    float VelY =  y - prev_y;
                    cube.FluidCubeAddVelocity(x,y, VelX, VelY);

                    prev_x = x; prev_y = y;


            }
        }
        cube.FluidCubeStep();


        window.clear();
        texture.update(pixels);
        sprite.setTexture(texture);
        window.draw(sprite);
        window.display();
    }
}