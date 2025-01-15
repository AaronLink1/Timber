// Timber.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <SFML/Graphics.hpp>
#include <sstream>
#include "Bee.h"
#include "Cloud.h"

using namespace sf;

//Function declarations
void centerOrigin(Text&);
void updateBranches();

enum class side { LEFT = 0, RIGHT, NONE };

const int NUM_BEES = 3;
const int NUM_CLOUDS = 5;
const int NUM_BRANCHES = 6;

//Setup branches arrays
Sprite branches[NUM_BRANCHES];
side branchSides[NUM_BRANCHES];

int main()
{
    //Seed random num generator and declare variables
    srand((int)time(0));
    Event event;
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

    //Load branch texture on GPU memory
    Texture textureBranch;
    textureBranch.loadFromFile("graphics/branch.png");

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

    //Init tree branches
    for (side& side : branchSides)
    {
        int random = rand() % 5;
        switch (random)
        {
        case 0:
            side = side::LEFT;
            break;
        case 1:
            side = side::RIGHT;
            break;
        default:
            side = side::NONE;
            break;
        }
    }

    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000, -2000);
        branches[i].setOrigin(220, 20);
        if (branchSides[i] == side::LEFT)
            branches[i].setRotation(180);
    }

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
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case Event::KeyPressed:
                switch (event.key.code)
                {
                case Keyboard::Escape:
                    window.close();
                    break;
                case Keyboard::Return:
                    paused = !paused;
                    break;
                case Keyboard::Left:
                    updateBranches();
                    break;
                case Keyboard::Right:
                    updateBranches();
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        }

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

            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                float height = i * 150;

                if (branchSides[i] == side::LEFT)
                    branches[i].setPosition(610, height);
                else if (branchSides[i] == side::RIGHT)
                    branches[i].setPosition(1330, height);
                else
                    branches[i].setPosition(3000, height);
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

        for (Sprite branch : branches)
            window.draw(branch);

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

void updateBranches()
{
    for (int i = NUM_BRANCHES - 1; i > 0; i--)
        branches[i] = branches[i - 1];

    for (int i = NUM_BRANCHES - 1; i > 0; i--)
        branchSides[i] = branchSides[i - 1];

    int random = rand() % 5;
    switch (random)
    {
    case 0:
        branchSides[0] = side::LEFT;
        branches[0].setRotation(180);
        break;
    case 1:
        branchSides[0] = side::RIGHT;
        branches[0].setRotation(0);
        break;
    default:
        branchSides[0] = side::NONE;
        break;
    }
}