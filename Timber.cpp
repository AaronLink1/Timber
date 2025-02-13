// Timber.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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

    //Load player texture on GPU memory and init player
    Texture texturePlayer;
    texturePlayer.loadFromFile("graphics/player.png");
    Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(580, 720); //Player starts on left side
    side playerSide = side::LEFT;
    
    //Load gravestone texture on GPU memory and init gravestone
    Texture textureRIP;
    textureRIP.loadFromFile("graphics/rip.png");
    Sprite spriteRIP;
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(675, 2000);

    //Load axe texture on GPU memory and init axe
    Texture textureAxe;
    textureAxe.loadFromFile("graphics/axe.png");
    Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(2000, 830);
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    //Load flying log texture on GPU memory and init flying log
    Texture textureLog;
    textureLog.loadFromFile("graphics/log.png");
    Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(810, 720);
    bool logActive = false;
    float logSpeedX = 1000, logSpeedY = -1500;

    //Load sounds onto RAM and associate with a sound object
    SoundBuffer bufferChop;
    bufferChop.loadFromFile("sound/chop.wav");
    Sound soundChop;
    soundChop.setBuffer(bufferChop);

    SoundBuffer bufferDeath;
    bufferDeath.loadFromFile("sound/death.wav");
    Sound soundDeath;
    soundDeath.setBuffer(bufferDeath);

    SoundBuffer bufferOOT;
    bufferOOT.loadFromFile("sound/out_of_time.wav");
    Sound soundOOT;
    soundOOT.setBuffer(bufferOOT);

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
        side = side::NONE;

    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000, -2000);
        branches[i].setOrigin(220, 20);
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
                        messageText.setString("PRESS ENTER TO RESUME!");
                        centerOrigin(messageText);

                        if (timeRemaining <= 0 || playerSide == branchSides[5])
                        {
                            timeRemaining = 6.0f;
                            score = 0;
                            
                            for (side& branch : branchSides)
                                branch = side::NONE;

                            spriteRIP.setPosition(675, 2000);
                            spritePlayer.setPosition(580, 720);
                            side playerSide = side::LEFT;
                        }
                    break;
                case Keyboard::Left:
                    if (!paused)
                    {
                        //Play sound
                        soundChop.play();

                        //Update player and axe
                        playerSide = side::LEFT;
                        spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
                        spritePlayer.setPosition(580, 720);

                        //Update score, time and branches
                        score++;
                        timeRemaining += 2.0f / score + 0.15;
                        updateBranches();

                        //Update flying log
                        spriteLog.setPosition(810, 720);
                        logSpeedX = 5000;
                        logActive = true;
                    }
                    break;
                case Keyboard::Right:
                    if (!paused)
                    {
                        //Play sound
                        soundChop.play();

                        //Update player and axe
                        playerSide = side::RIGHT;
                        spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
                        spritePlayer.setPosition(1200, 720);

                        //Update score, time and branches
                        score++;
                        timeRemaining += 2.0f / score + 0.15;
                        updateBranches();

                        //Update flying log
                        spriteLog.setPosition(810, 720);
                        logSpeedX = -5000;
                        logActive = true;
                    }
                    break;
                }
                break;
            case Event::KeyReleased:
                spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
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

            if (logActive)
            {
                spriteLog.setPosition(spriteLog.getPosition().x + logSpeedX * dt.asSeconds(), spriteLog.getPosition().y + logSpeedY * dt.asSeconds());
                if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000)
                {
                    logActive = false;
                    spriteLog.setPosition(810, 720);
                }
            }

            if (branchSides[5] == playerSide)
            {
                paused = true;
                spriteRIP.setPosition(spritePlayer.getPosition());
                spritePlayer.setPosition(2000, 660);
                messageText.setString("YOU DIED! PRESS ENTER TO RESTART!");
                centerOrigin(messageText);
                soundDeath.play();
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
            {
                paused = true;
                messageText.setString("PRESS ENTER TO RESTART!");
                centerOrigin(messageText);
                soundOOT.play();
            }
        }

        //Draw the Scene
        window.clear();

        window.draw(spriteBackground); //Draw background

        for (Cloud& cloud : clouds) //Draw clouds
            window.draw(cloud.getSprite());

        for (Sprite branch : branches)
            window.draw(branch);

        window.draw(spriteTree); //Draw tree

        window.draw(spritePlayer); //Draw player

        window.draw(spriteAxe); //Draw axe

        window.draw(spriteLog); //Draw log

        window.draw(spriteRIP); //Draw gravestone

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