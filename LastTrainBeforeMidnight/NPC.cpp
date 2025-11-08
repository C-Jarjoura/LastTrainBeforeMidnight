#include "NPC.h"
#include <cmath>

NPC::NPC(const std::string& textureFile, sf::Vector2f pos)
    : m_texture(textureFile)
    , m_sprite(m_texture)
{
    m_speed = 0.8f;    // breathing speed
    m_position = pos;
    m_sprite.setPosition(m_position);

    // first idle frame
    m_sprite.setTextureRect(
        sf::IntRect(
            sf::Vector2i(0, 0),
            sf::Vector2i(259, 328)
        )
    );
}

void NPC::setScale(float s)
{
    m_sprite.setScale(sf::Vector2f{ s,s });
}

void NPC::update(float dt)
{
    // breathing like player idle
    m_animTimer += dt;
    float a = sin(m_animTimer * m_speed) * 1.5f; // small up/down
    m_sprite.setPosition(m_position + sf::Vector2f{ 0.f, a });
}

void NPC::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}
