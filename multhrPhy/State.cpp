#include "State.h"
#include <chrono>

State::State(sf::RenderWindow& window, sf::Font& font)
	:window{ window }, font{ font }, window_size{ this->window.getSize() }, total_dt{ 0 }, update_times{ 0 }, lock{}
{

	this->logical_size = sf::Vector2f{ 1600.f,1000.f };//初始化大小

	g = sf::Vector2f(0.f, 100.f);//重力加速度

	this->background.setFillColor(sf::Color::White);
	this->background.setPosition(0.f, 0.f);
	this->background.setSize(this->window_size);//background同时代表显示区域大小

	this->average_dt.setFillColor(sf::Color::Black);
	this->average_dt.setFont(this->font);
	this->average_dt.setPosition(sf::Vector2f(0, 0));

	circle_num = 200;
	frame_cnt = 0;
	// 分帧数量，可以根据当前帧数进行调整，
	// 例如目标是30fps，当前能达到120fps，那就可以调整到cut_num = 4
	cut_num = 4;
	now_ts = std::chrono::high_resolution_clock::now();

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
		this->objects.push_back(new Circle(mass_dist(gen), init_vel, rand_pos, radius, color));
	}

	for (auto& i : this->objects)
	{
		Circle* c = dynamic_cast<Circle*>(i);
		this->SAP_on_x.push_back(
			SAP{ c,
			c->getLogicalPosition().x + c->getLogicalRadius(),
			c->getLogicalPosition().x - c->getLogicalRadius()
			});

		this->SAP_on_y.push_back(
			SAP{ c,
			c->getLogicalPosition().y + c->getLogicalRadius(),
			c->getLogicalPosition().y - c->getLogicalRadius()
			});
	}

	this->collision_pairs.resize((unsigned int)(this->objects.size() * this->objects.size() * 0.02));

}

State::~State()
{
	for (auto& i : this->objects)
	{
		delete i;
	}
	omp_destroy_lock(&this->lock);
}

void State::updateGravity(const float& dt)
{
	int j = ceil(circle_num / cut_num) * frame_cnt;
	int k = ceil(circle_num / cut_num) * (frame_cnt + 1);
	if (k > circle_num)
		k = circle_num;
	for (; j < k; j++)
	{
		objects[j]->appendVelocity(this->g * dt);
	}
}

void State::updateBoundary()
{
	int j = ceil(circle_num / cut_num) * frame_cnt;
	int k = ceil(circle_num / cut_num) * (frame_cnt + 1);
	if (k > circle_num)
		k = circle_num;
	for (; j < k; j++)
	{
		Circle* cir = dynamic_cast<Circle*>(objects[j]);
		sf::Vector2f pos = cir->getLogicalPosition();
		sf::Vector2f vel = cir->getVelocity();
		float rad = cir->getLogicalRadius();
		if (pos.y + rad > this->logical_size.y)
		{
			if (vel.y > 0.f)
				cir->setVelocity(sf::Vector2f(vel.x, -1.f * vel.y));
		}
		if (pos.y + rad < 0.f)
		{
			if (vel.y < 0.f)
				cir->setVelocity(sf::Vector2f(vel.x, -1.f * vel.y));
		}
		if (pos.x + rad > this->logical_size.x)
		{
			if (vel.x > 0.f)
				cir->setVelocity(sf::Vector2f(-1.f * vel.x, vel.y));
		}
		if (pos.x - rad < 0.f)
		{
			if (vel.x < 0.f)
				cir->setVelocity(sf::Vector2f(-1.f * vel.x, vel.y));
		}
	}
}

void State::updateCollision()
{
	auto startTime = std::chrono::high_resolution_clock::now();

	if (frame_cnt % cut_num == 0)
		this->broadPhase();
	this->narrowPhase();
	this->resolvePhase();

	frame_cnt = (frame_cnt + 1) % cut_num;

	auto endTime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
	std::cout << "updateCollision execution time: " << duration << " microseconds" << std::endl;

}

void State::broadPhase()
{
	this->possible_on_x.clear();
	this->possible_on_y.clear();
	this->possible_collision_pairs.clear();

#pragma omp parallel
	{
#pragma omp single
		{
			detectAxises();
		}
	}

	intersectAxises();
}

void State::detectAxises() {
	double start = omp_get_wtime();

#pragma omp task
	detectAxisX();

#pragma omp task
	detectAxisY();

	double end = omp_get_wtime();
	std::printf("detectAxises execution time:%.16g microseconds\n", (end - start) * 1000);
}

void State::detectAxisX() {
	sf::Vector2f v1, v2;
	float m1, m2;
	int len = this->SAP_on_x.size();

	//#pragma omp parallel for
	for (int i{ 0 }; i < len; i++)
	{
		Circle* cx = dynamic_cast<Circle*>(this->SAP_on_x[i].pointer);
		v1 = cx->getLogicalPosition();
		m1 = cx->getLogicalRadius();
		this->SAP_on_x[i].lower = v1.x - m1;
		this->SAP_on_x[i].upper = v1.x + m1;
	}

	for (int i{ 1 }; i < len; i++)
	{
		int j{ i - 1 };
		SAP k{ this->SAP_on_x[i] };
		while ((j >= 0) && (k.lower < this->SAP_on_x[j].lower))
		{
			this->SAP_on_x[j + 1] = this->SAP_on_x[j];
			j--;
		}
		this->SAP_on_x[j + 1] = k;
	}

	//omp_init_lock(&this->lock);
//#pragma omp parallel for
	for (int i{ 0 }; i < len; i++)
	{
		int j{ i + 1 };
		while ((j < len) && (this->SAP_on_x[i].upper > this->SAP_on_x[j].lower))
		{
			//omp_set_lock(&lock);
			this->possible_on_x.push_back
			(this->SAP_on_x[i].pointer->serial_number > this->SAP_on_x[j].pointer->serial_number
				? std::make_pair(this->SAP_on_x[i].pointer, this->SAP_on_x[j].pointer)
				: std::make_pair(this->SAP_on_x[j].pointer, this->SAP_on_x[i].pointer));
			//omp_unset_lock(&lock);
			j++;
		}
	}
}

void State::detectAxisY() {
	sf::Vector2f v1, v2;
	float m1, m2;
	int len = this->SAP_on_x.size();

	//#pragma omp parallel for
	for (int i{ 0 }; i < len; i++)
	{
		Circle* cy = dynamic_cast<Circle*>(this->SAP_on_y[i].pointer);
		v2 = cy->getLogicalPosition();
		m2 = cy->getLogicalRadius();
		this->SAP_on_y[i].lower = v2.y - m2;
		this->SAP_on_y[i].upper = v2.y + m2;
	}

	for (int i{ 1 }; i < len; i++)
	{
		int j{ i - 1 };
		SAP k{ this->SAP_on_y[i] };
		while ((j >= 0) && (k.lower < this->SAP_on_y[j].lower))
		{
			this->SAP_on_y[j + 1] = this->SAP_on_y[j];
			j--;
		}
		this->SAP_on_y[j + 1] = k;
	}

	//omp_init_lock(&this->lock);
//#pragma omp parallel for
	for (int i{ 0 }; i < len; i++)
	{
		int j{ i + 1 };
		while ((j < len) && (this->SAP_on_y[i].upper > this->SAP_on_y[j].lower))
		{
			//omp_set_lock(&lock);
			this->possible_on_y.push_back
			(this->SAP_on_y[i].pointer->serial_number > this->SAP_on_y[j].pointer->serial_number
				? std::make_pair(this->SAP_on_y[i].pointer, this->SAP_on_y[j].pointer)
				: std::make_pair(this->SAP_on_y[j].pointer, this->SAP_on_y[i].pointer));
			//omp_unset_lock(&lock);
			j++;
		}
	}
}

void State::intersectAxises() {
	double start = omp_get_wtime();

	int x{ (int)this->possible_on_x.size() };
	int y{ (int)this->possible_on_y.size() };

#pragma omp parallel
	{
		std::vector<std::pair<Object*, Object*>> vec_private;
#pragma omp for nowait
		for (int i{ 0 }; i < x; i++)
		{
			for (int j{ 0 }; j < y; j++)
			{
				if (this->possible_on_x[i].first == this->possible_on_y[j].first &&
					this->possible_on_x[i].second == this->possible_on_y[j].second)
				{
					vec_private.push_back(this->possible_on_x[i]);
					break;
				}
			}
		}

#pragma omp critical
		this->possible_collision_pairs.insert(this->possible_collision_pairs.end(), vec_private.begin(), vec_private.end());
	}

	double end = omp_get_wtime();
	std::printf("intersectAxises execution time:%.16g microseconds\n", (end - start) * 1000);
}

void State::narrowPhase()
{
	this->collision_pairs.clear();
	int obj_cnt = (int)this->possible_collision_pairs.size();
	float dis;
	int j = ceil(obj_cnt / cut_num) * frame_cnt;
	int k = ceil(obj_cnt / cut_num) * (frame_cnt + 1);

	omp_init_lock(&this->lock);
#pragma omp parallel for

	for (int i{ j }; i < k; i++)
	{
		Circle* c1 = dynamic_cast<Circle*>(this->possible_collision_pairs[i].first);
		Circle* c2 = dynamic_cast<Circle*>(this->possible_collision_pairs[i].second);
		dis = MathFunction::point_point_dis(c1->getLogicalPosition(), c2->getLogicalPosition());
		if (dis < c1->getLogicalRadius() + c2->getLogicalRadius())
		{
			this->dis_pairs[std::make_pair(c1, c2)] = dis;
			omp_set_lock(&this->lock);
			this->collision_pairs.push_back(std::make_pair(c1, c2));
			omp_unset_lock(&this->lock);
		}
	}
}

void State::resolvePhase()
{
	int obj_cnt = (int)this->collision_pairs.size();

	int j = ceil(obj_cnt / cut_num) * frame_cnt;
	int k = ceil(obj_cnt / cut_num) * (frame_cnt + 1);

	omp_init_lock(&this->lock);
#pragma omp parallel for
	for (int i = j; i < k; i++)
	{
		Circle* c1 = dynamic_cast<Circle*>(this->collision_pairs[i].first);
		Circle* c2 = dynamic_cast<Circle*>(this->collision_pairs[i].second);
		float dist = this->dis_pairs[std::make_pair(c1, c2)];

		omp_set_lock(&this->lock);
		float m1 = c1->getMass();
		float m2 = c2->getMass();
		sf::Vector2f v1 = c1->getVelocity();
		sf::Vector2f v2 = c2->getVelocity();
		sf::Vector2f c1_to_c2 = c2->getLogicalPosition() - c1->getLogicalPosition();
		sf::Vector2f unit_c1c2 = c1_to_c2 / dist;

		// 计算碰撞前的动量
		sf::Vector2f p1 = m1 * v1;
		sf::Vector2f p2 = m2 * v2;

		// 根据动量守恒定理计算碰撞后的速度
		sf::Vector2f v1_new = (p1 + 2.0f * m2 / (m1 + m2) * MathFunction::scalar_product(p2 - p1, unit_c1c2) * unit_c1c2) / (m1);
		sf::Vector2f v2_new = (p2 - 2.0f * m1 / (m1 + m2) * MathFunction::scalar_product(p2 - p1, unit_c1c2) * unit_c1c2) / (m2);

		c1->setVelocity(v1_new);
		c2->setVelocity(v2_new);
		omp_unset_lock(&this->lock);
	}
}


void State::update(const float& dt)
{
	this->updateBoundary();
	this->updateCollision();
	this->updateGravity(dt);

	for (auto& i : this->objects)
	{
		i->update(dt, this->background.getSize(), this->logical_size, sf::Vector2f(0, 0));
	}
	std::chrono::high_resolution_clock::time_point cur = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = std::chrono::duration_cast<std::chrono::duration<double>>(cur - now_ts);
	if (duration.count() > 1) {
		this->average_dt.setString(std::to_string(int(1 / dt)) + "FPS");
		now_ts = cur;
	}
}

void State::render(sf::RenderTarget& target)
{
	target.draw(this->background);

	for (auto& i : this->objects)
	{
		i->render(target);
	}

	target.draw(this->average_dt);
}
