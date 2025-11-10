#include "Player.h"
#include <SFML/Window/Keyboard.hpp>

Player::Player()
    : m_speed(200.f),
    m_currentFrame(0),
    m_animTimer(0.f),
    m_texture("assets/sprites/player.png"),
    m_sprite(m_texture)
{
    setPosition({ 960.f,540.f });
    m_lastPosition = getPosition();

    m_sprite.setTextureRect(
        sf::IntRect(
            sf::Vector2i(0, 0),
            sf::Vector2i(FRAME_W, FRAME_H)
        )
    );
}

void Player::setScale(float s)
{
    m_sprite.setScale(sf::Vector2f{ s,s });
}

void Player::setPosition(const sf::Vector2f& pos)
{
    Entity::setPosition(pos);
    m_sprite.setPosition(pos);
}


void Player::update(float dt)
{
    m_lastPosition = m_position;
    sf::Vector2f m(0, 0);

    bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D);
    bool left = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A);

    if (right) m.x += m_speed * dt;
    if (left)  m.x -= m_speed * dt;

    auto pos = getPosition();
    pos += m;
    setPosition(pos);

    m_animTimer += dt;

    if (right) { if (m_animTimer > 0.12f) { m_animTimer = 0; m_currentFrame = (m_currentFrame == 2) ? 3 : 2; } }
    else if (left) { if (m_animTimer > 0.12f) { m_animTimer = 0; m_currentFrame = (m_currentFrame == 4) ? 5 : 4; } }
    else { if (m_animTimer > 0.35f) { m_animTimer = 0; m_currentFrame = (m_currentFrame == 0) ? 1 : 0; } }

    m_sprite.setTextureRect(
        sf::IntRect(
            sf::Vector2i(m_currentFrame * FRAME_W, 0),
            sf::Vector2i(FRAME_W, FRAME_H)
        )
    );
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}
