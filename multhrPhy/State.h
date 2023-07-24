#ifndef STATE_H
#define STATE_H

#include"Circle.h"

class State
{
private:

	sf::Vector2f g;

	sf::RectangleShape background;

	unsigned int screen_width;
	unsigned int screen_height;

	float dotProduct(sf::Vector2f v1, sf::Vector2f v2);
public:
	State();
	~State();

	void updateGravity(const float& dt);//向所有物体施加重力加速度
	void updateBoundary();
	void updateCollision();

	void update(const float& dt);
	void render(sf::RenderTarget& target);
};

#endif