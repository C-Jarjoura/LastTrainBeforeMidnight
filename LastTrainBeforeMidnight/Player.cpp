#include "Player.h"

Player::Player()
{
    m_speed = 200.f;

    // Charge texture joueur (tu dois avoir un fichier player.png dans le dossier du .exe)
    m_texture.loadFromFile("player.png");
    m_sprite.setTexture(m_texture);

    m_position = { 960.f, 540.f }; // centre écran 1920x1080
    m_sprite.setPosition(m_position);
}

void Player::update(float dt)
{
    sf::Vector2f movement(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) // Z (AZERTY)
        movement.y -= m_speed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S))
        movement.y += m_speed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)) // Q (AZERTY)
        movement.x -= m_speed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D))
        movement.x += m_speed * dt;

    m_position += movement;
    m_sprite.setPosition(m_position);
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}