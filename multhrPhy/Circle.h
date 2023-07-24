#ifndef CIRCLE_H
#define CIRCLE_H

#include "Object.h"

class Circle :
    public Object
{
private:
    sf::Vector2f position;
    float radius;

    sf::CircleShape circle;

public:
    Circle(const float& mass, const sf::Vector2f velocity, const sf::Vector2f& position, const float& radius, const int& color_index);
    ~Circle();

    const sf::Vector2f& getPosition();
    const float& getRadius();

    void setPosition(const sf::Vector2f& position);
    void setRadius(const float& radius);

    virtual const std::string shapeType();

    virtual void updateMovement(const float& dt);
    virtual void updateCollision();
    virtual void updateLogic();

    virtual void render(sf::RenderTarget& target);
};

#endif