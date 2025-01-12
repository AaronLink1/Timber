#include "Cloud.h"

Cloud::Cloud()
{
	speed = 0.0f;
	active = false;
}

void Cloud::setSprite(const sf::Texture& texture)
{
	spriteCloud.setTexture(texture);
}

void Cloud::setPosition(float x, float y)
{
	spriteCloud.setPosition(x, y);
}

void Cloud::setSpeed(float speed)
{
	this->speed = speed;
}

void Cloud::setActive(bool active)
{
	this->active = active;
}

void Cloud::move(float dt)
{
	float x = spriteCloud.getPosition().x + (speed * dt);
	spriteCloud.setPosition(x, spriteCloud.getPosition().y);
}

bool Cloud::offScreen()
{
	if (spriteCloud.getPosition().x > 1920)
		return true;

	return false;
}
