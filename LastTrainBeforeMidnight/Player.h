#pragma once

#include <SFML/Graphics.hpp>
#include "Entity.h"

class Player : public Entity
{
public:
    Player();

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

private:
    float m_speed;
    sf::Sprite m_sprite;
    sf::Texture m_texture;
};