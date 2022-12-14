//
// Created by Brian Cheng on 14/11/2022.
//

#include "FluidCube2D.h"
#include "iostream"
#include "math.h"
#include "SFML/Graphics.hpp"
int w = SIZE;
int h = SIZE;

int upper_bound = SIZE-2;
int lower_bound = 2;

int main()
{
//    mode, window title name, style(what functions to include in title bar)
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Window", sf::Style::Close | sf::Style::Titlebar); //| sf::Style::Resize);
    window.setFramerateLimit(120);

//    model parameters
    FluidCube2D cube(0.00000000000001,0, 0.0005);

//    create pixel array
    sf::Texture texture;
    texture.create(w,h);

    sf::Sprite sprite(texture);
    sprite.scale(WINDOW_SIZE / SIZE, WINDOW_SIZE / SIZE);
    sf::Uint8* pixels =  (sf::Uint8*) calloc(h * w * 4, sizeof(float));



// create loop for window to stay open
    float last_max =  255;
    int prev_x = 0;
    int prev_y = 0;

    while (window.isOpen()){
        for (int i=0; i< h; i++){
            for (int j=0; j<w; j++){
//                RGB set as constant 255
                pixels[(i*w + j)*4] = pixels[(i*w + j)*4 +1] = pixels[(i*w + j)*4 +2] = (sf::Uint8)255;

//                deal with strange pixel behavior, possibly caused by random trash values during declaration
                if (isnan(cube.density[i*w + j])){cube.density[i*w + j] = 0;}
                if (prev_x==0 and prev_y == 0 and cube.density[i*w + j] != 0){cube.density[i*w + j] = 0;}

//                pixel density used as A in RGBA
                int level = (int)(cube.density[i*w + j]);
                pixels[(i*w + j)*4 +3] = (sf::Uint8)std::min(abs(level), 255);
            }
        }




//        window activities
        sf::Event evnt;
        while (window.pollEvent(evnt)){
//        close window case
            switch(evnt.type){
                case sf::Event::Closed:
                    window.close();
                    break;
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
                sf::Vector2i pos = sf::Mouse::getPosition(window);
                int pixel_x = int(float(pos.x)/WINDOW_SIZE * w);
                int pixel_y = int(float(pos.y)/WINDOW_SIZE * h);

                int x = std::max(lower_bound, std::min(pixel_x, upper_bound));
                int y = std::max(lower_bound, std::min(pixel_y, upper_bound));

//              add density when muose pressed
                cube.FluidCubeAddDensity(x, y , SIZE*10);

                int VelX =  pos.x - prev_x;
                int VelY =  pos.y - prev_y;

//              add velocity when mouse dragged
                int SCALE = 50;
                cube.FluidCubeAddVelocity(x,y, SCALE * VelX, SCALE * VelY);

                prev_x = pos.x; prev_y = pos.y;
            }

        }

        cube.FluidCubeStep();
        cube.fadeout();

        window.clear();
        texture.update(pixels);
        sprite.setTexture(texture);
        window.draw(sprite);
        window.display();
    }
}