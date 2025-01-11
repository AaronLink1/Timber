// Timber.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
    //Create and open a window for the game
    RenderWindow window(VideoMode(1920, 1080), "Timber", Style::Fullscreen);

    //GameLoop
    while (window.isOpen())
    {
        //Handle player input
        if (Keyboard::isKeyPressed(Keyboard::Escape))
            window.close();

        //Update the Scene

        //Draw the Scene
        window.clear();

        window.display();
    }

    return 0;
}
