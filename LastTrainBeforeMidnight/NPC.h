#pragma once

#include <SFML/Graphics.hpp>
#include "Entity.h"

class NPC : public Entity
{
public:
    NPC(const std::string& textureFile, bool canMove, bool moveVertical, sf::Vector2f pos);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

private:
    bool m_moveVertical; // true = vertical move, false = horizontal
    bool m_canMove;
    float m_speed;

    sf::Texture m_texture;
    sf::Sprite m_sprite;

    float m_moveTimer = 0.f;
};
