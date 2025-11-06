#include "Player.h"

Player::Player()
    : m_speed(200.f),
    m_texture("assets/Player/player.png"),
    m_sprite(m_texture)
{
    m_position = { 960.f, 540.f };
    m_sprite.setPosition(m_position);

    // First frame (col=0,row=0)
    m_sprite.setTextureRect(sf::IntRect(sf::Vector2i{ 0, 0 }, sf::Vector2i{ 128, 128 }));
}

void Player::update(float dt)
{
    sf::Vector2f movement(0.f, 0.f);

    bool up = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W);
    bool down = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S);
    bool left = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A);
    bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D);

    if (up)    movement.y -= m_speed * dt;
    if (down)  movement.y += m_speed * dt;
    if (left)  movement.x -= m_speed * dt;
    if (right) movement.x += m_speed * dt;

    // choose row (column stays 0 for now)
    if (up)
        m_sprite.setTextureRect(sf::IntRect(sf::Vector2i{ 0, 2 * 128 }, sf::Vector2i{ 128, 128 }));
    else if (down)
        m_sprite.setTextureRect(sf::IntRect(sf::Vector2i{ 0, 0 * 128 }, sf::Vector2i{ 128, 128 }));
    else if (right)
        m_sprite.setTextureRect(sf::IntRect(sf::Vector2i{ 0, 1 * 128 }, sf::Vector2i{ 128, 128 }));
    else if (left)
        m_sprite.setTextureRect(sf::IntRect(sf::Vector2i{ 0, 3 * 128 }, sf::Vector2i{ 128, 128 }));

    m_position += movement;
    m_sprite.setPosition(m_position);
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}
