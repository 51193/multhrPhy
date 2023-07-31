#include "MathFunction.h"

const float MathFunction::_sqrt(float num)
{
    float x2{ num * 0.5F }, y{ num };
    const float threehalfs = 1.5F;

    long i{ *(long*)&y };
    i = 0x5f3759df - (i >> 1);

    y = *(float*)&i;
    y = y * (threehalfs - (x2 * y * y));

    return 1.f / y;
}

const float MathFunction::point_point_dis(const sf::Vector2f& point1, const sf::Vector2f& point2)
{
    sf::Vector2f len{point1 - point2};
    return MathFunction::_sqrt(len.x * len.x + len.y * len.y);
    //return sqrtf((point1.x - point2.x) * (point1.x - point2.x) + (point1.y - point2.y) * (point1.y - point2.y));
}

const bool MathFunction::circle_circle_jud(const sf::Vector2f& position1, const sf::Vector2f& position2, const float& radius1, const float& radius2)
{
    float dis = point_point_dis(position1 + sf::Vector2f(radius1, radius1), position2 + sf::Vector2f(radius2, radius2));
    if (dis > radius1 + radius2)
        return false;
    return true;
}

const float MathFunction::scalar_product(const sf::Vector2f& vector1, const sf::Vector2f& vector2)
{
    return vector1.x * vector2.x + vector1.y * vector2.y;
}
