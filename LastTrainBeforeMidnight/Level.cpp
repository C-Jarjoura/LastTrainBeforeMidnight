#include "Level.h"
#include <SFML/Window/Keyboard.hpp>

static inline bool aabbOverlap(const sf::Rect<float>& a, const sf::Rect<float>& b)
{
    return (a.position.x < b.position.x + b.size.x) &&
        (a.position.x + a.size.x > b.position.x) &&
        (a.position.y < b.position.y + b.size.y) &&
        (a.position.y + a.size.y > b.position.y);
}

Level::Level()
    : m_scene1("assets/scenes/scene_1.png")
    , m_panelTex("assets/sprites/panel.png")
    , m_sceneSprite(m_scene1)
    , m_panelSprite(m_panelTex)
{
    m_sceneSprite.setPosition({ 0.f,0.f });

    m_panelSprite.setPosition({ 750.f,385.f });
    m_panelSprite.setScale({ 0.5f,0.5f }); // <<< reduction ici

    // et la zone de collision, on adapte selon le scale
    m_panelZone = sf::Rect<float>(
        sf::Vector2f{ 750.f,385.f },
        sf::Vector2f{ 150.f * 0.5f,120.f * 0.5f }  // <<< même réduction
    );
}


void Level::update(float dt)
{
    m_player.update(dt);

    sf::Vector2f p = m_player.getPosition();
    sf::Rect<float> pRect(p, { 32.f,32.f });

    if (aabbOverlap(pRect, m_panelZone))
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::E))
        {
            // plus tard -> changer de station
        }
    }
}

void Level::draw(sf::RenderWindow& window)
{
    window.draw(m_sceneSprite);
    window.draw(m_panelSprite);
    m_player.draw(window);
}
