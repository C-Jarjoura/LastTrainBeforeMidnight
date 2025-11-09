#pragma once
#include <SFML/Graphics.hpp>

class ScreenFader
{
public:
    ScreenFader();

    void startFadeOut();
    void startFadeIn();

    void update(float dt);
    void draw(sf::RenderWindow& window);

    bool isBlack() const { return (m_alpha >= 255.f); }
    bool done()   const { return !m_active; }

    // NOUVEAU
    void setColor(const sf::Color& c);

private:
    sf::RectangleShape m_rect;
    float m_alpha;
    float m_speed;
    bool  m_active;
    bool  m_isOut;

    sf::Color m_color;  // NOUVEAU
};
