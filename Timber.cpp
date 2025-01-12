// Timber.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
    //Create and open a window for the game
    RenderWindow window(VideoMode(1920, 1080), "Timber", Style::Fullscreen);

    //Load the background texture on the GPU and attach to a sprite
    Texture textureBackground;
    textureBackground.loadFromFile("graphics/background.png");
    Sprite spriteBackground;
    spriteBackground.setTexture(textureBackground);
    spriteBackground.setPosition(0, 0);

    //GameLoop
    while (window.isOpen())
    {
        //Handle player input
        if (Keyboard::isKeyPressed(Keyboard::Escape))
            window.close();

        //Update the Scene

        //Draw the Scene
        window.clear();

        window.draw(spriteBackground);

        window.display();
    }

    return 0;
}
