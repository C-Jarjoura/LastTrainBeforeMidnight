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
    sf::Texture m_texture;   // <-- texture first
    sf::Sprite  m_sprite;    // <-- sprite after
};
