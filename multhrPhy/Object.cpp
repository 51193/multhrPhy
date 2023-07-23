#include "Object.h"

std::vector<Object*>Object::objects;

Object::Object(const float& mass, const sf::Vector2f velocity)
	:mass{ mass }, velocity{ velocity }
{
}

Object::~Object()
{
}

const float& Object::getMass()
{
	return this->mass;
}

const sf::Vector2f& Object::getVelocity()
{
	return this->velocity;
}

void Object::appendVelocity(const sf::Vector2f& append_velocity)
{
	this->velocity += append_velocity;
}

void Object::setVelocity(const sf::Vector2f& set_velocity)
{
	this->velocity = set_velocity;
}

void Object::update(const float& dt)
{
	this->updateCollision();
	this->updateMovement(dt);
	this->updateLogic();
}
