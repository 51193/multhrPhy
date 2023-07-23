#ifndef OBJECT_H
#define OBJECT_H

#include<vector>
#include<iostream>

#include"CollisionFunction.h"

class Object
{
protected:
	float mass;
	sf::Vector2f velocity;
public:
	static std::vector<Object*>objects;

	Object(const float& mass, const sf::Vector2f velocity);
	~Object();

	const float& getMass();
	const sf::Vector2f& getVelocity();
	void appendVelocity(const sf::Vector2f& append_velocity);
	void setVelocity(const sf::Vector2f& set_velocity);

	virtual const std::string shapeType() = 0;

	void update(const float& dt);

	virtual void updateMovement(const float& dt) = 0;
	virtual void updateCollision() = 0;
	virtual void updateLogic() = 0;

	virtual void render(sf::RenderTarget& target) = 0;
};

#endif