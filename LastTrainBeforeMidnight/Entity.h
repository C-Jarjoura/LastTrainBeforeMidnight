#pragma once

#include <SFML/Graphics.hpp>

class Entity
{
public:
    virtual ~Entity() = default;

    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void setPosition(const sf::Vector2f& pos) { m_position = pos; }
    sf::Vector2f getPosition() const { return m_position; }

protected:
    sf::Vector2f m_position{};
};