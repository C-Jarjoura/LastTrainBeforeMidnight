#include "Player.h"

Player::Player()
    : m_speed(200.f),
    m_texture("player.png"),   // SFML 3: construct texture from file
    m_sprite(m_texture)        // SFML 3: construct sprite with texture
{
    m_position = { 960.f, 540.f }; // center for 1920x1080
    m_sprite.setPosition(m_position);
}

void Player::update(float dt)
{
    sf::Vector2f movement(0.f, 0.f);

    // ZQSD (via scancodes: W,A,S,D on the keyboard)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) movement.y -= m_speed * dt; // Z
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) movement.y += m_speed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)) movement.x -= m_speed * dt; // Q
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)) movement.x += m_speed * dt;

    m_position += movement;
    m_sprite.setPosition(m_position);
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}
