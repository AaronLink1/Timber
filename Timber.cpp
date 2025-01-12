// Timber.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <SFML/Graphics.hpp>
#include "Bee.h"

using namespace sf;

const int NUM_BEES = 5;

int main()
{
    //Seed random num generator and declare variables
    srand((int)time(0));
    Clock clock;
    std::vector<Bee> bees((rand() % NUM_BEES) + 1);

    //Create and open a window for the game
    RenderWindow window(VideoMode(1920, 1080), "Timber", Style::Fullscreen);

    //Load the background texture on the GPU and attach to a sprite
    Texture textureBackground;
    textureBackground.loadFromFile("graphics/background.png");
    Sprite spriteBackground;
    spriteBackground.setTexture(textureBackground);
    spriteBackground.setPosition(0, 0);

    //Load bee texture on GPU memory
    Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");

    //Init Bees
    for (Bee& bee : bees)
    {
        bee.setSprite(textureBee);
        bee.setPosition(0, 0);
        bee.setActive(false);
    }

    //GameLoop
    while (window.isOpen())
    {
        //Handle player input
        if (Keyboard::isKeyPressed(Keyboard::Escape))
            window.close();

        //Update the Scene
        Time dt = clock.restart();

        for (Bee& bee : bees)
        {
            if (!bee.isActive())
            {
                bee.setPosition(2000, (rand() % 500) + 500);
                bee.setSpeed((rand() % 150) + 200);
                bee.setActive(true);
            }
            else
            {
                bee.move(dt.asSeconds());
                if (bee.offScreen())
                    bee.setActive(false);
            }
        }

        //Draw the Scene
        window.clear();

        window.draw(spriteBackground); //Draw background

        for (Bee& bee : bees) //Draw Bees
            window.draw(bee.getSprite());

        window.display();

    }
    return 0;
}
