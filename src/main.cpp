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
    window.setFramerateLimit(10);

    FluidCube cube(0.1,0, 0.0001);

    sf::Texture texture;
    texture.create(w,h);

    sf::Sprite sprite(texture);
    sprite.scale(WINDOW_SIZE / SIZE, WINDOW_SIZE / SIZE);
    sf::Uint8* pixels = new sf::Uint8 [h * w * 4];

// create loop for window to stay open
    float last_max =  255;
    while (window.isOpen()){

        float current_max=0;

        for (int i=0; i< h; i++){
            for (int j=0; j<w; j++){

                current_max = cube.density[i*w + j] > current_max?  cube.density[i+w+j]: current_max;

//                pixels[(i*w + j)*4]    = (sf::Uint8)(std::min(int(255*(cube.density[i*w + j] / last_max)), 255));
//                pixels[(i*w + j)*4 +1] = (sf::Uint8)(std::min(int(255*(cube.density[i*w + j] / last_max)), 255));
//                pixels[(i*w + j)*4 +2] = (sf::Uint8)(std::min(int(255*(cube.density[i*w + j] / last_max)), 255));

//                pixels[(i*w + j)*4]    = (sf::Uint8)std::max(int(cube.density[i*w + j]), 50);
//                pixels[(i*w + j)*4 +1] = (sf::Uint8)std::max(int(cube.density[i*w + j]), 50);
//                pixels[(i*w + j)*4 +2] = (sf::Uint8)std::max(int(cube.density[i*w + j]), 50);

                pixels[(i*w + j)*4]    = (sf::Uint8)255;
                pixels[(i*w + j)*4 +1] = (sf::Uint8)255;
                pixels[(i*w + j)*4 +2] = (sf::Uint8)255;

                pixels[(i*w + j)*4 +3] = cube.density[i*w + j];
            }
        }

        int prev_x = 0;
        int prev_y = 0;


//        window activities
        sf::Event evnt;
        while (window.pollEvent(evnt)){
//        close window case
            switch(evnt.type){
                case sf::Event::Closed:
                    window.close();
                    break;
            }
            sf::Vector2i pos = sf::Mouse::getPosition(window);
            int x = pos.x;
            int y = pos.y;

            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
                cube.FluidCubeAddDensity(int(float(x)/WINDOW_SIZE * w), int(float(y)/WINDOW_SIZE * h) , 200);

                int VelX =  x - prev_x;
                int VelY =  y - prev_y;
                int SCALE = 1000;
                cube.FluidCubeAddVelocity(int(float(x)/WINDOW_SIZE * w),int(float(y)/WINDOW_SIZE * h), SCALE * VelX, SCALE * VelY);
//                    printf("%d, %d\n", int(float(x)/WINDOW_SIZE * w), int(float(y)/WINDOW_SIZE * h));
//                std::cout << cube.density[int(float(x)/WINDOW_SIZE * w) + w* int(float(y)/WINDOW_SIZE * h)] << std::endl;
                prev_x = x; prev_y = y;
            }

        }
        cube.FluidCubeStep();

        window.clear();
        texture.update(pixels);
        sprite.setTexture(texture);
        window.draw(sprite);
        window.display();

        last_max = current_max;
        current_max = 0;
    }
}