#include "State.h"

State::State(sf::RenderWindow& window, sf::Font& font)
	:window{ window }, font{ font }, window_size{ this->window.getSize() }
{

	this->logical_size = sf::Vector2f{ 1600.f,1000.f };//初始化大小

	g = sf::Vector2f(0.f, 1000.f);//重力加速度

	this->background.setFillColor(sf::Color::White);
	this->background.setPosition(0.f, 0.f);
	this->background.setSize(this->window_size);//background同时代表显示区域大小

	new Circle(10.f, sf::Vector2f(500.f, 0.f), sf::Vector2f(10.f, 10.f), 10.f);
}

State::~State()
{
	for (auto& i : Object::objects)
	{
		if (i)delete i;
	}
	Object::objects.clear();
}

void State::updateGravity(const float& dt)
{
	for (auto& i : Object::objects)
	{
		i->appendVelocity(this->g * dt);
	}
}

void State::updateBoundary()
{
	for (auto& i : Object::objects)
	{
		if (i->shapeType() == "circle")
		{
			if (dynamic_cast<Circle*>(i)->getLogicalPosition().y + dynamic_cast<Circle*>(i)->getLogicalRadius() > this->logical_size.y)
			{
				if (i->getVelocity().y > 0.f)
					i->setVelocity(sf::Vector2f(i->getVelocity().x, -1.f * i->getVelocity().y));
			}
			if (dynamic_cast<Circle*>(i)->getLogicalPosition().y + dynamic_cast<Circle*>(i)->getLogicalRadius() < 0.f)
			{
				if (i->getVelocity().y < 0.f)
					i->setVelocity(sf::Vector2f(i->getVelocity().x, -1.f * i->getVelocity().y));
			}
			if (dynamic_cast<Circle*>(i)->getLogicalPosition().x + dynamic_cast<Circle*>(i)->getLogicalRadius() > this->logical_size.x)
			{
				if (i->getVelocity().x > 0.f)
					i->setVelocity(sf::Vector2f(-1.f * i->getVelocity().x, i->getVelocity().y));
			}
			if (dynamic_cast<Circle*>(i)->getLogicalPosition().x - dynamic_cast<Circle*>(i)->getLogicalRadius() < 0.f)
			{
				if (i->getVelocity().x < 0.f)
					i->setVelocity(sf::Vector2f(-1.f * i->getVelocity().x, i->getVelocity().y));
			}
		}
	}
}

void State::update(const float& dt)
{
	this->updateBoundary();
	this->updateGravity(dt);

	for (auto& i : Object::objects)
	{
		i->update(dt, this->background.getSize(), this->logical_size, sf::Vector2f(0, 0));
	}
}

void State::render(sf::RenderTarget& target)
{
	target.draw(this->background);

	for (auto& i : Object::objects)
	{
		i->render(target);
	}
}
