#include "State.h"
#include <random>
#include <windows.h> 

State::State()
{

	g = sf::Vector2f(0.f, 1000.f);//重力加速度

	screen_width = sf::VideoMode::getDesktopMode().width;
	screen_height = sf::VideoMode::getDesktopMode().height;

	this->background.setFillColor(sf::Color::White);
	this->background.setPosition(0.f, 0.f);
	this->background.setSize(sf::Vector2f((float)screen_width, (float)screen_height));

	int circle_num = 200;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> pos_dist(0.f, screen_height);
	std::uniform_real_distribution<float> vel_dist(-500.f, 500.f);
	std::uniform_real_distribution<float> radius_dist(5.f, 15.f);
	std::uniform_real_distribution<float> color_dist(0, 7);

	for (int i = 0; i < circle_num; ++i)
	{
		sf::Vector2f init_vel(vel_dist(gen), vel_dist(gen));
		float radius = radius_dist(gen);
		sf::Vector2f rand_pos(pos_dist(gen), pos_dist(gen));
		int color = int(color_dist(gen));
		new Circle(10.f, init_vel, rand_pos, radius, color);
	}
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
			if (dynamic_cast<Circle*>(i)->getPosition().y + dynamic_cast<Circle*>(i)->getRadius() > screen_height)
			{
				if (i->getVelocity().y > 0.f)
					i->setVelocity(sf::Vector2f(i->getVelocity().x, -1.f * i->getVelocity().y));
			}
			if (dynamic_cast<Circle*>(i)->getPosition().y - dynamic_cast<Circle*>(i)->getRadius() < 0.f)
			{
				if (i->getVelocity().y < 0.f)
					i->setVelocity(sf::Vector2f(i->getVelocity().x, -1.f * i->getVelocity().y));
			}
			if (dynamic_cast<Circle*>(i)->getPosition().x + dynamic_cast<Circle*>(i)->getRadius() > screen_width)
			{
				if (i->getVelocity().x > 0.f)
					i->setVelocity(sf::Vector2f(-1.f * i->getVelocity().x, i->getVelocity().y));
			}
			if (dynamic_cast<Circle*>(i)->getPosition().x - dynamic_cast<Circle*>(i)->getRadius() < 0.f)
			{
				if (i->getVelocity().x < 0.f)
					i->setVelocity(sf::Vector2f(-1.f * i->getVelocity().x, i->getVelocity().y));
			}
		}
	}
}

void State::updateCollision()
{
	size_t obj_num = Object::objects.size();
	for (int i = 0; i < obj_num; ++i)
	{
		for (int j = i + 1; j < obj_num; ++j)
		{
			Circle* c1 = dynamic_cast<Circle*>(Object::objects[i]);
			Circle* c2 = dynamic_cast<Circle*>(Object::objects[j]);
			float dist = std::hypot(c2->getPosition().y - c1->getPosition().y, c2->getPosition().x - c1->getPosition().x);
			if (dist <= c1->getRadius() + c2->getRadius())
			{
				sf::Vector2f v1 = c1->getVelocity();
				sf::Vector2f v2 = c2->getVelocity();
				sf::Vector2f c1_to_c2 = c2->getPosition() - c1->getPosition();

				sf::Vector2f unit_c1c2 = c1_to_c2 / dist;
				sf::Vector2f v1_parallel = unit_c1c2 * dotProduct(v1, unit_c1c2);
				sf::Vector2f v1_perpendicular = v1 - v1_parallel;
				sf::Vector2f v2_parallel = unit_c1c2 * dotProduct(v2, unit_c1c2);
				sf::Vector2f v2_perpendicular = v2 - v2_parallel;
				sf::Vector2f v1_new = v2_parallel + v1_perpendicular;
				sf::Vector2f v2_new = v1_parallel + v2_perpendicular;
				c1->setVelocity(v1_new);
				c2->setVelocity(v2_new);
			}
		}
	}
}

// 计算向量的点积
float State::dotProduct(sf::Vector2f v1, sf::Vector2f v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

void State::update(const float& dt)
{
	this->updateBoundary();
	this->updateCollision();
	this->updateGravity(dt);

	for (auto& i : Object::objects)
	{
		i->update(dt);
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
