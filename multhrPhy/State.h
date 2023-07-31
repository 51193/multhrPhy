#ifndef STATE_H
#define STATE_H

#include<random>
#include<omp.h>
#include<algorithm>
#include <chrono>
#include <ctime>

#include"Circle.h"

struct SAP
{
	Object* pointer;
	float upper;
	float lower;

	SAP(Object* pointer, float upper, float lower)
		: pointer(pointer), upper(upper), lower(lower)
	{
	}

	const bool operator<(const SAP& x)
	{
		return lower < x.lower;
	}

	const bool operator>(const SAP& x)
	{
		return lower > x.lower;
	}
};

class State
{
private:
	omp_lock_t lock;

	sf::RenderWindow& window;
	sf::Font& font;

	sf::Vector2f window_size;//这个变量和window的声明顺序不能调换，涉及到初始化
	sf::Vector2f logical_size;

	sf::Vector2f g;

	sf::RectangleShape background;

	std::vector<Object*>objects;

	std::vector<SAP>SAP_on_x;
	std::vector<SAP>SAP_on_y;

	std::vector<std::pair<Object*, Object*>> possible_on_x;
	std::vector<std::pair<Object*, Object*>> possible_on_y;
	std::vector<std::pair<Object*, Object*>> possible_collision_pairs;

	std::map<std::pair<Object*, Object*>, float> dis_pairs;

	std::vector<std::pair<Object*, Object*>> collision_pairs;

	unsigned int update_times;
	float total_dt;
	sf::Text average_dt;

	// 分帧处理
	int circle_num;
	int frame_cnt;
	int cut_num;
	std::chrono::high_resolution_clock::time_point now_ts;

	void updateCollision();

	void broadPhase();
	void narrowPhase();
	void resolvePhase();

public:
	State(sf::RenderWindow& window, sf::Font& font);
	~State();

	void updateGravity(const float& dt);//向所有物体施加重力加速度
	void updateBoundary();//根据边界进行反弹，后面会修改，现在是直接写死的

	void update(const float& dt);
	void render(sf::RenderTarget& target);
};

#endif