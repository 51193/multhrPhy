#ifndef MATHFUNCTION_H
#define MATHFUNCTION_H

#include<string>

#include<SFML/Graphics.hpp>

class MathFunction
{
public:
	static const float _sqrt(float num);

	static const float point_point_dis(const sf::Vector2f& point1, const sf::Vector2f& point2);

	static const bool circle_circle_jud(const sf::Vector2f& position1, const sf::Vector2f& position2, const float& radius1, const float& radius2);

	static const float scalar_product(const sf::Vector2f& vector1, const sf::Vector2f& vector2);//俩向量的内积
};

#endif