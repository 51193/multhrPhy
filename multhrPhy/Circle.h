#ifndef CIRCLE_H
#define CIRCLE_H

#include "Object.h"

class Circle :
    public Object
{
private:
    sf::Vector2f logical_position;
    sf::Vector2f rendering_position;
    float logical_radius;
    float rendering_radius;

    sf::CircleShape circle;

public:
    Circle(const float& mass, const sf::Vector2f velocity, const sf::Vector2f& position, const float& radius);
    ~Circle();

    const sf::Vector2f& getLogicalPosition();
    const sf::Vector2f& getRenderingPosition();
    const float& getLogicalRadius();
    const float& getRenderingRadius();

    void setLogicalPosition(const sf::Vector2f& position);
    void setRenderingPosition(const sf::Vector2f& position);
    void setLogicalRadius(const float& radius);
    void setRenderingRadius(const float& radius);

    virtual const std::string shapeType();

    virtual void updateMovement(const float& dt);
    virtual void updateShape(const sf::Vector2f& rendering_size, const sf::Vector2f& logical_size, const sf::Vector2f& offset);
    virtual void updateCollision();
    virtual void updateLogic();

    virtual void render(sf::RenderTarget& target);
};

#endif