#pragma once
#include <SFML/Graphics.hpp>

class ScreenFader
{
public:
    ScreenFader();

    void startFadeOut(); // vers noir (durée ~0.6)
    void startFadeIn();  // depuis noir (durée ~0.6)

    void update(float dt);
    void draw(sf::RenderWindow& window);

    bool active() const { return m_active; }
    bool done()   const { return !m_active; }
    bool isBlack() const { return m_alpha >= 255; }

private:
    float m_alpha;
    float m_speed;
    bool  m_active;
    bool  m_isOut; // true = fadeOut / false = fadeIn

    sf::RectangleShape m_rect;
};
