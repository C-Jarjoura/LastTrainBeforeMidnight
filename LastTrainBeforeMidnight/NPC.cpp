#include "NPC.h"
#include <cmath>

NPC::NPC(const std::string& textureFile, bool canMove, bool moveVertical, sf::Vector2f pos)
    : m_texture(textureFile),
    m_sprite(m_texture)
{
    m_canMove = canMove;
    m_moveVertical = moveVertical;
    m_speed = 50.f;

    m_position = pos;
    m_sprite.setPosition(m_position);
}

void NPC::update(float dt)
{
    if (m_canMove)
    {
        m_moveTimer += dt;
        float offset = sin(m_moveTimer * m_speed) * 100.f; // amplitude 100 pixels

        if (m_moveVertical)
            m_position.y += offset * dt;
        else
            m_position.x += offset * dt;

        m_sprite.setPosition(m_position);
    }
}

void NPC::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}
