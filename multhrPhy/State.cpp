#include "State.h"

void State::handleCollision()
{
	for (auto& i : Object::collision_pairs)
	{
		if (i.first->shapeType() == "circle" && i.second->shapeType() == "circle")
		{
			Circle* circle1 = dynamic_cast<Circle*>(i.first);
			Circle* circle2 = dynamic_cast<Circle*>(i.second);

			const sf::Vector2f& v1 = circle1->getVelocity();
			const sf::Vector2f& v2 = circle2->getVelocity();

			const sf::Vector2f& c1 = circle2->getLogicalPosition() - circle1->getLogicalPosition();
			const sf::Vector2f& c2 = circle1->getLogicalPosition() - circle2->getLogicalPosition();

			if (
				MathFunction::scalar_product(v1, c1) / MathFunction::point_point_dis(sf::Vector2f(0.f, 0.f), c1)
				+
				MathFunction::scalar_product(v2, c2) / MathFunction::point_point_dis(sf::Vector2f(0.f, 0.f), c2)
				> 0.f
				)
			{
				float m1 = circle1->getMass();
				float m2 = circle2->getMass();

				circle1->setVelocity(sf::Vector2f(
					((m1 - m2) * v1.x + 2 * m2 * v2.x) / (m1 + m2),
					((m1 - m2) * v1.y + 2 * m2 * v2.y) / (m1 + m2)
				));
				circle2->setVelocity(sf::Vector2f(
					((m2 - m1) * v2.x + 2 * m1 * v1.x) / (m1 + m2),
					((m2 - m1) * v2.y + 2 * m1 * v1.y) / (m1 + m2)
				));//这俩式子是我拿动量定理跟动能定理联立解出来的，不知道为什么不能用
			}
		}
	}
}

State::State(sf::RenderWindow& window, sf::Font& font)
	:window{ window }, font{ font }, window_size{ this->window.getSize() }
{

	this->logical_size = sf::Vector2f{ 1600.f,1000.f };//初始化大小

	g = sf::Vector2f(0.f, 100.f);//重力加速度

	this->background.setFillColor(sf::Color::White);
	this->background.setPosition(0.f, 0.f);
	this->background.setSize(this->window_size);//background同时代表显示区域大小

	int circle_num = 100;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> pos_dist(0.f, this->window_size.y);
	std::uniform_real_distribution<float> vel_dist(-500.f, 500.f);
	std::uniform_real_distribution<float> radius_dist(5.f, 15.f);
	std::uniform_real_distribution<float> color_dist(0, 7);
	std::uniform_real_distribution<float> mass_dist(5, 15);

	for (int i = 0; i < circle_num; ++i)
	{
		sf::Vector2f init_vel(vel_dist(gen), vel_dist(gen));
		float radius = radius_dist(gen);
		sf::Vector2f rand_pos(pos_dist(gen), pos_dist(gen));
		int color = int(color_dist(gen));
		new Circle(mass_dist(gen), init_vel, rand_pos, radius, color);
	}
}

State::~State()
{
	for (auto& i : Object::objects)
	{
		//delete i;这地方delete了会有个delete_scaller报错，暂时没搞明白什么情况
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

void State::updateCollision()
{
	size_t obj_num = Object::objects.size();
	for (int i = 0; i < obj_num; ++i)
	{
		for (int j = i + 1; j < obj_num; ++j)
		{
			Circle* c1 = dynamic_cast<Circle*>(Object::objects[i]);
			Circle* c2 = dynamic_cast<Circle*>(Object::objects[j]);
			float dist = std::hypot(c2->getLogicalPosition().y - c1->getLogicalPosition().y, c2->getLogicalPosition().x - c1->getLogicalPosition().x);
			if (dist <= c1->getLogicalRadius() + c2->getLogicalRadius())
			{
				sf::Vector2f v1 = c1->getVelocity();
				sf::Vector2f v2 = c2->getVelocity();
				sf::Vector2f c1_to_c2 = c2->getLogicalPosition() - c1->getLogicalPosition();

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

	//Object::collision_pairs.clear();

	for (auto& i : Object::objects)
	{
		i->update(dt, this->background.getSize(), this->logical_size, sf::Vector2f(0, 0));
	}

	//this->handleCollision();
}

void State::render(sf::RenderTarget& target)
{
	target.draw(this->background);

	for (auto& i : Object::objects)
	{
		i->render(target);
	}
}
