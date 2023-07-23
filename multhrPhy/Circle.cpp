#include "Circle.h"

Circle::Circle(const float& mass, const sf::Vector2f velocity, const sf::Vector2f& position, const float& radius)
	:Object{ mass,velocity }, position{ position }, radius{ radius }
{
	Object::objects.push_back(this);
	this->circle.setFillColor(sf::Color::Black);
	this->circle.setOutlineColor(sf::Color::Black);
	this->circle.setPosition(this->position);
	this->circle.setRadius(this->radius);
}

Circle::~Circle()
{
}

const sf::Vector2f& Circle::getPosition()
{
	return this->position;
}

const float& Circle::getRadius()
{
	return this->radius;
}

void Circle::setPosition(const sf::Vector2f& position)
{
	this->position = position;
}

void Circle::setRadius(const float& radius)
{
	this->radius = radius;
}

const std::string Circle::shapeType()
{
	return "circle";
}

void Circle::updateMovement(const float& dt)
{
	this->position += this->velocity * dt;
	this->circle.setPosition(this->position);
}

void Circle::updateCollision()
{
	//别重复判定同一个物体
}

void Circle::updateLogic()
{
}

void Circle::render(sf::RenderTarget& target)
{
	target.draw(this->circle);
}
