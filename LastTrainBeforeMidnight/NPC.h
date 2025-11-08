#pragma once

#include <SFML/Graphics.hpp>
#include "Entity.h"

class NPC : public Entity
{
public:
    NPC(const std::string& textureFile, sf::Vector2f pos);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    void setScale(float s); // <<=== AJOUT

private:
    float m_speed;
    float m_animTimer = 0.f;

    sf::Texture m_texture;
    sf::Sprite  m_sprite;
};
