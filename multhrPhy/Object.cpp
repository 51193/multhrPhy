#include "Object.h"

std::vector<Object*>Object::objects; 
std::vector<std::pair<Object*, Object*>>Object::collision_pairs;

void Object::handleCollision()
{
	for (auto& i : Object::collision_pairs)
	{
		if (i.first->shapeType() == "circle" && i.second->shapeType() == "circle")
		{
			Circle* circle1 = dynamic_cast<Circle*>(i.first);
			Circle* circle2 = dynamic_cast<Circle*>(i.second);

			const sf::Vector2f& velocity1 = circle1->getVelocity();
			const sf::Vector2f& velocity2 = circle2->getVelocity();


		}
	}
}

const bool Object::judgeCollision(Object* object1, Object* object2)
{
	if (object1->shapeType() == "circle" && object2->shapeType() == "circle")
	{
		Circle* circle1 = dynamic_cast<Circle*>(object1);
		Circle* circle2 = dynamic_cast<Circle*>(object2);
		return 
			CollisionFunction::circle_circle_jud
			(circle1->getLogicalPosition(), circle2->getLogicalPosition(), circle1->getLogicalRadius(), circle2->getLogicalRadius());
	}
}

Object::Object(const float& mass, const sf::Vector2f& velocity)
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

void Object::update(const float& dt, const sf::Vector2f& rendering_size, const sf::Vector2f& logical_size, const sf::Vector2f& offset)
{
	this->updateCollision();
	this->updateMovement(dt);
	this->updateLogic();
	this->updateShape(rendering_size, logical_size, offset);
}
