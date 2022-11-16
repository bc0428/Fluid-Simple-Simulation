//
// Created by Brian Cheng on 14/11/2022.
//

#include <unistd.h>
#include "FluidCube2D.h"
#include "iostream"
#include "math.h"
#include "SFML/Graphics.hpp"
int w = SIZE;
int h = SIZE;

int main()
{
//    mode, window title name, style(what functions to include in title bar)
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Window", sf::Style::Close | sf::Style::Titlebar); //| sf::Style::Resize);
    window.setFramerateLimit(6);

    FluidCube cube(1,1,0.00000000001);

    sf::Texture texture;
    texture.create(w,h);

    sf::Sprite sprite(texture);
    sprite.scale(WINDOW_SIZE / SIZE, WINDOW_SIZE / SIZE);
    sf::Uint8* pixels = new sf::Uint8 [h * w * 4];

// create loop for window to stay open
    while (window.isOpen()){


        for (int i=0; i< h; i++){
            for (int j=0; j<w; j++){
                pixels[(i*w + j)*4]    = (sf::Uint8)(std::min(cube.density[i*w + j], 255));
                pixels[(i*w + j)*4 +1] = (sf::Uint8)(std::min(cube.density[i*w + j], 255));
                pixels[(i*w + j)*4 +2] = (sf::Uint8)(std::min(cube.density[i*w + j], 255));
                pixels[(i*w + j)*4 +3] = (sf::Uint8)255;
            }
        }

        float prev_x = 0;
        float prev_y = 0;


//        window activities
        sf::Event evnt;
        while (window.pollEvent(evnt)){
//        close window case
            switch(evnt.type){
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseButtonPressed:
                    sf::Vector2i pos = sf::Mouse::getPosition(window);
                    int x = pos.x;
                    int y = pos.y;

                    cube.FluidCubeAddDensity(int(float(x)/WINDOW_SIZE * w), int(float(y)/WINDOW_SIZE * h) , 100);
                    int VelX =  x - prev_x;
                    int VelY =  y - prev_y;
//                    cube.FluidCubeAddVelocity(int(float(x)/WINDOW_SIZE * w),int(float(y)/WINDOW_SIZE * h), VelX, VelY);

                    printf("%d, %d\n", int(float(x)/WINDOW_SIZE * w), int(float(y)/WINDOW_SIZE * h));
//                    cube.density[int(float(x)/WINDOW_SIZE * w) + w* int(float(y)/WINDOW_SIZE * h)] = 255;

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