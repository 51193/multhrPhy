#include "Circle.h"

Circle::Circle(const float& mass, const sf::Vector2f& velocity, const sf::Vector2f& position, const float& radius)
	:Object{ mass,velocity }, logical_position{ position }, logical_radius{ radius }
{
	this->circle.setFillColor(sf::Color::Black);
	this->circle.setOutlineColor(sf::Color::Black);
	this->circle.setRadius(this->logical_radius);
}

Circle::~Circle()
{
}

const sf::Vector2f& Circle::getLogicalPosition()
{
	return this->logical_position;
}

const sf::Vector2f& Circle::getRenderingPosition()
{
	return this->rendering_position;
}

const float& Circle::getLogicalRadius()
{
	return this->logical_radius;
}

void Circle::setLogicalPosition(const sf::Vector2f& position)
{
	this->logical_position = position;
}

void Circle::setRenderingPosition(const sf::Vector2f& position)
{
	this->rendering_position = position;
}

void Circle::setLogicalRadius(const float& radius)
{
	this->logical_radius = radius;
}

const std::string Circle::shapeType()
{
	return "circle";
}

void Circle::updateMovement(const float& dt)
{
	this->logical_position += this->velocity * dt;
}

void Circle::updateShape(const sf::Vector2f& rendering_size, const sf::Vector2f& logical_size, const sf::Vector2f& offset)
{
	float x = rendering_size.x / logical_size.x;
	float y = rendering_size.y / logical_size.y;

	this->rendering_position.x = this->logical_position.x * x + offset.x;
	this->rendering_position.y = this->logical_position.y * y + offset.y;

	this->circle.setScale(sf::Vector2f(x, y));
	this->circle.setPosition(this->rendering_position);
}

void Circle::updateCollision()
{
	bool flag{ false };
	for (size_t i{ 0 }; i < Object::objects.size(); i++)
	{
		if (flag)
		{

			if (Object::objects[i]->shapeType() == "circle")
			{
				Circle* circle = dynamic_cast<Circle*>(Object::objects[i]);
				if (MathFunction::circle_circle_jud
				(this->getLogicalPosition(), circle->getLogicalPosition(), this->getLogicalRadius(), circle->getLogicalRadius())
					)
				{
					Object::collision_pairs.push_back(std::make_pair(this, Object::objects[i]));
				}
			}
		}
		else if (Object::objects[i] = this)
		{
			flag = true;
		}
	}
}

void Circle::updateLogic()
{
}

void Circle::render(sf::RenderTarget& target)
{
	target.draw(this->circle);
}
