// Timber.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <SFML/Graphics.hpp>
#include <sstream>
#include "Bee.h"
#include "Cloud.h"

using namespace sf;

void centerOrigin(Text&);

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
    std::stringstream ss;
    int score = 0;

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

    //Load font and create text variables
    Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");

    Text scoreText, messageText;
    scoreText.setFont(font); //Setup score text
    scoreText.setFillColor(Color::White);
    scoreText.setCharacterSize(75);
    ss << "Score: " << score;
    scoreText.setString(ss.str());
    scoreText.setPosition(20, 20);

    messageText.setFont(font); //Setup message text
    messageText.setFillColor(Color::White);
    messageText.setCharacterSize(100);
    messageText.setString("PRESS ENTER TO START!");
    centerOrigin(messageText);
    messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

    RectangleShape timebar; //Setup timebar
    float timebarWidth = 400;
    float timebarHeight = 80;
    timebar.setSize(Vector2f(timebarWidth, timebarHeight));
    timebar.setFillColor(Color::Red);
    timebar.setPosition(960 - timebarWidth / 2, 980);

    Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timebarWidthPerSecond = timebarWidth / timeRemaining;

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

            //Update score
            ss.str(std::string());
            ss << "Score: " << score;
            scoreText.setString(ss.str());

            //Update timebar
            timeRemaining -= dt.asSeconds();
            timebar.setSize(Vector2f(timebarWidthPerSecond * timeRemaining, timebarHeight));
            timebar.setPosition(960 - timebarWidthPerSecond * timeRemaining / 2, 980);

            if (timeRemaining <= 0.0f)
                paused = true;
        }

        //Draw the Scene
        window.clear();

        window.draw(spriteBackground); //Draw background

        for (Cloud& cloud : clouds) //Draw clouds
            window.draw(cloud.getSprite());

        window.draw(spriteTree); //Draw tree

        for (Bee& bee : bees) //Draw bees
            window.draw(bee.getSprite());

        window.draw(scoreText); //Draw score
 
        window.draw(timebar); //Draw timebar

        if (paused)
            window.draw(messageText); //Draw message

        window.display();

    }
    return 0;
}

void centerOrigin(Text& text)
{
    FloatRect tempRect = text.getLocalBounds();
    text.setOrigin(tempRect.left + tempRect.width / 2, tempRect.top + tempRect.height / 2);
}
