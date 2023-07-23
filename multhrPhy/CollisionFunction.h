#ifndef COLLISIONFUNCTION_H
#define COLLISIONFUNCION_H

#include<string>

#include<SFML/Graphics.hpp>

class CollisionFunction
{
public:
	static const float point_point_dis(const sf::Vector2f& point1, const sf::Vector2f& point2);

	static const bool circle_circle_jud(const sf::Vector2f& position1, const sf::Vector2f& position2, const float& radius1, const float& radius2);
};

#endif