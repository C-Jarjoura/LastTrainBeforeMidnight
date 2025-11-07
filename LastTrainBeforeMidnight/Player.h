#pragma once
#include <SFML/Graphics.hpp>

class Player
{
public:
    Player();

    void update(float dt);
    void draw(sf::RenderWindow& window);

    sf::Vector2f getPosition() const { return m_position; }
    sf::Vector2f getLastPosition() const { return m_lastPosition; }
    void setPosition(const sf::Vector2f& pos);

private:
    sf::Texture m_texture;
    sf::Sprite  m_sprite;

    float       m_speed;
    sf::Vector2f m_position;
    sf::Vector2f m_lastPosition;

    int   m_currentFrame;
    float m_animTimer;

    static constexpr int FRAME_W = 259;
    static constexpr int FRAME_H = 328;
};
