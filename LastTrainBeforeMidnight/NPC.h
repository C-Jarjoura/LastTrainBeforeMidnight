#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "Entity.h"

class NPC : public Entity
{
public:
    NPC(const std::string& textureFile, const sf::Vector2f& pos);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    void setScale(float s);
    sf::Vector2f getPosition() const { return Entity::getPosition(); }
    void setPosition(const sf::Vector2f& pos) override;


private:
    sf::Texture m_texture;
    sf::Sprite  m_sprite;


    float m_animTimer = 0.f;
    int   m_currentFrame = 0;

    // CONSTANT FRAME SIZE (2 frames idle @259x328)
    static constexpr int FRAME_W = 259;
    static constexpr int FRAME_H = 328;
};
