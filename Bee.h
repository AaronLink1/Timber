#pragma once
#include <SFML/Graphics.hpp>
class Bee
{
public:
	//Constructor
	Bee();

	//Functions
	void setSprite(const sf::Texture&);
	void setPosition(float, float);
	void setSpeed(float);
	void setActive(bool);
	void move(float);
	bool offScreen();
	bool isActive() { return active; }
	sf::Sprite& getSprite() { return beeSprite; }

private:
	sf::Sprite beeSprite;
	float speed;
	bool active;
};

