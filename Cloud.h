#pragma once
#include <SFML/Graphics.hpp>
class Cloud
{
public:
	//Constructor
	Cloud();

	//Functions
	void setSprite(const sf::Texture&);
	void setPosition(float, float);
	void setSpeed(float);
	void setActive(bool);
	void move(float);
	bool offScreen();
	bool isActive() { return active; }
	sf::Sprite& getSprite() { return spriteCloud; }

private:
	sf::Sprite spriteCloud;
	float speed;
	bool active;
};

