#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class NPC
{
public:
    NPC(const std::string& textureFile, const sf::Vector2f& pos);

    void update(float dt);
    void draw(sf::RenderWindow& window);

    void setScale(float s);
    sf::Vector2f getPosition() const { return m_position; }

private:
    sf::Texture m_texture;
    sf::Sprite  m_sprite;

    sf::Vector2f m_position;

    float m_animTimer = 0.f;
    int   m_currentFrame = 0;

    // CONSTANT FRAME SIZE (2 frames idle @259x328)
    static constexpr int FRAME_W = 259;
    static constexpr int FRAME_H = 328;
};
