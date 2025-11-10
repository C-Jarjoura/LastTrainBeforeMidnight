#include "NPC.h"
#include <cmath>
NPC::NPC(const std::string& textureFile, const sf::Vector2f& pos)
    : m_texture(textureFile)
    , m_sprite(m_texture)
{
    setPosition(pos);


    // start idle frame 0
    m_sprite.setTextureRect(sf::IntRect({ 0,0 }, { FRAME_W,FRAME_H }));
}

void NPC::setScale(float s)
{
    m_sprite.setScale({ s,s });
}
void NPC::setPosition(const sf::Vector2f& pos)
{
    Entity::setPosition(pos);
    m_sprite.setPosition(pos);
}
void NPC::update(float dt)
{
    m_animTimer += dt;

    // 2-frame idle breathing
    if (m_animTimer > 0.8f)
    {
        m_animTimer = 0.f;
        m_currentFrame = (m_currentFrame == 0) ? 1 : 0;

        m_sprite.setTextureRect(
            sf::IntRect({ m_currentFrame * FRAME_W, 0 },
                { FRAME_W, FRAME_H })
        );
    }

    // breathing vertical offset small oscillation
    float breathe = std::sin(m_animTimer * 2.0f) * 1.5f;
    auto pos = getPosition();
    m_sprite.setPosition({ pos.x, pos.y + breathe });
}

void NPC::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}
