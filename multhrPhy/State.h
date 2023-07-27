#ifndef STATE_H
#define STATE_H

#include<random>

#include"Circle.h"

class State
{
private:

	sf::RenderWindow& window;
	sf::Font& font;

	sf::Vector2f window_size;//这个变量和window的声明顺序不能调换，涉及到初始化
	sf::Vector2f logical_size;

	sf::Vector2f g;

	sf::RectangleShape background;

	void handleCollision();

	float dotProduct(sf::Vector2f v1, sf::Vector2f v2);

public:
	State(sf::RenderWindow& window, sf::Font& font);
	~State();

	void updateGravity(const float& dt);//向所有物体施加重力加速度
	void updateBoundary();//根据边界进行反弹，后面会修改，现在是直接写死的

	void updateCollision();
	void update(const float& dt);
	void render(sf::RenderTarget& target);
};

#endif