#ifndef STATE_H
#define STATE_H

#include"Circle.h"

class State
{
private:

	sf::Vector2f g;

	sf::RectangleShape background;

public:
	State();
	~State();

	void updateGravity(const float& dt);
	void updateBoundary();

	void update(const float& dt);
	void render(sf::RenderTarget& target);
};

#endif