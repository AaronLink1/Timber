#include "Bee.h"

Bee::Bee()
{
	speed = 0.0f;
	active = false;
}

void Bee::setSprite(const sf::Texture& texture)
{
	beeSprite.setTexture(texture);
}

void Bee::setPosition(float x, float y)
{
	beeSprite.setPosition(x, y);
}

void Bee::setSpeed(float speed)
{
	this->speed = speed;
}

void Bee::setActive(bool active)
{
	this->active = active;
}

void Bee::move(float dt)
{
	beeSprite.setPosition(beeSprite.getPosition().x - (speed * dt), beeSprite.getPosition().y);
}

bool Bee::offScreen()
{
	if (beeSprite.getPosition().x < -100)
		return true;

	return false;
}