// Timber.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <SFML/Graphics.hpp>
#include "Bee.h"
#include "Cloud.h"

using namespace sf;

const int NUM_BEES = 3;
const int NUM_CLOUDS = 5;

int main()
{
    //Seed random num generator and declare variables
    srand((int)time(0));
    Clock clock;
    std::vector<Bee> bees((rand() % NUM_BEES) + 1);
    std::vector<Cloud> clouds((rand() % NUM_CLOUDS) + 3);
    bool paused = true;

    //Create and open a window for the game
    RenderWindow window(VideoMode(1920, 1080), "Timber", Style::Fullscreen);

    //Load the background texture on the GPU and attach to a sprite
    Texture textureBackground;
    textureBackground.loadFromFile("graphics/background.png");
    Sprite spriteBackground;
    spriteBackground.setTexture(textureBackground);
    spriteBackground.setPosition(0, 0);

    //Load tree texture on GPU memory
    Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");
    Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(810, 0);

    //Load bee texture on GPU memory
    Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");

    //Load cloud texture on GPU memory
    Texture textureCloud;
    textureCloud.loadFromFile("graphics/cloud.png");

    //Init bees
    for (Bee& bee : bees)
    {
        bee.setSprite(textureBee);
        bee.setPosition(0, 0);
        bee.setActive(false);
    }

    //Init clouds
    for (Cloud& cloud : clouds)
    {
        cloud.setSprite(textureCloud);
        cloud.setPosition(0, 0);
        cloud.setActive(false);
    }

    //Game loop
    while (window.isOpen())
    {
        //Handle player input
        if (Keyboard::isKeyPressed(Keyboard::Escape)) //Close the game
            window.close();

        if (Keyboard::isKeyPressed(Keyboard::Return)) //Pause/unpause the game
            paused = !paused;

        //Update the Scene
        Time dt = clock.restart();

        if (!paused)
        {
            for (Bee& bee : bees) //Update the bees
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

            for (Cloud& cloud : clouds) //Update the clouds
            {
                if (!cloud.isActive())
                {
                    cloud.setPosition(-300, rand() % 325);
                    cloud.setSpeed((rand() % 25) + 20);
                    cloud.setActive(true);
                }
                else
                {
                    cloud.move(dt.asSeconds());
                    if (cloud.offScreen())
                        cloud.setActive(false);
                }
            }
        }

        //Draw the Scene
        window.clear();

        window.draw(spriteBackground); //Draw background

        for (Cloud& cloud : clouds) //Draw clouds
            window.draw(cloud.getSprite());

        window.draw(spriteTree); //Draw tree

        for (Bee& bee : bees) //Draw bees
            window.draw(bee.getSprite());

        window.display();

    }
    return 0;
}
