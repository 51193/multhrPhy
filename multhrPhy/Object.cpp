#include "Object.h"

int Object::No;
//std::vector<Object*>Object::objects; 
//std::vector<std::pair<Object*, Object*>>Object::collision_pairs;

Object::Object(const float& mass, const sf::Vector2f& velocity)
	:mass{ mass }, velocity{ velocity }, serial_number{ No }
{
	//Object::objects.push_back(this);
	No++;
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

void Object::update(const float& dt, const sf::Vector2f& rendering_size, const sf::Vector2f& logical_size, const sf::Vector2f& offset)
{
	//this->updateCollision();
	this->updateMovement(dt);
	this->updateLogic();
	this->updateShape(rendering_size, logical_size, offset);
}
