#pragma once
#include <SFML/Graphics.hpp>

class Player
{
public:
    static constexpr int FRAME_W = 259;
    static constexpr int FRAME_H = 328;

    Player();

    void update(float dt);
    void draw(sf::RenderWindow& window);

    sf::Vector2f getPosition() const { return m_position; }
    void setPosition(const sf::Vector2f& pos);

    // nouveau -> pour changer scale selon scène
    void setScale(float s);

private:
    float m_speed;
    int   m_currentFrame;
    float m_animTimer;

    sf::Texture m_texture;
    sf::Sprite  m_sprite;

    sf::Vector2f m_position;
    sf::Vector2f m_lastPosition;
};
