#include "Level.h"
#include <SFML/Window/Keyboard.hpp>

static constexpr float PANEL_WIDTH = 150.f;
static constexpr float PANEL_HEIGHT = 120.f;

bool Level::aabbOverlap(const sf::Rect<float>& a, const sf::Rect<float>& b)
{
    return (a.position.x < b.position.x + b.size.x) &&
        (a.position.x + a.size.x > b.position.x) &&
        (a.position.y < b.position.y + b.size.y) &&
        (a.position.y + a.size.y > b.position.y);
}

Level::Level()
    : m_panelTex("assets/sprites/panel.png")
    , m_panelNext(m_panelTex)
    , m_panelPrev(m_panelTex)
    , m_font("assets/fonts/arial.ttf")
    , m_pressE(m_font, "", 28)
{
    m_sceneTexPtr = nullptr;
    m_hasNext = false;
    m_hasPrev = false;
    m_groundY = 540.f;

    m_pressE.setFillColor(sf::Color(255, 255, 255, 0));
}

void Level::setScene(int sceneId, const sf::Texture& sceneTexture)
{
    m_sceneTexPtr = &sceneTexture;

    const float H = 2000.f;

    m_hasNext = false;
    m_hasPrev = false;
    m_panelNext.setScale({ 1.f,1.f });
    m_panelPrev.setScale({ 1.f,1.f });

    if (sceneId == 1)
    {
        m_hasNext = true;
        m_groundY = 540.f;
        m_player.setScale(1.f);

        m_panelNext.setScale({ 0.45f,0.45f });
        m_panelNext.setPosition({ 600.f,400.f });
        m_zoneNext = sf::Rect<float>({ 610.f,140.f }, { PANEL_WIDTH * 0.45f,H });
    }
    else if (sceneId == 2)
    {
        m_hasPrev = true;
        m_hasNext = true;
        m_groundY = 540.f;
        m_player.setScale(1.f);

        m_panelPrev.setScale({ 0.45f,0.45f });
        m_panelPrev.setPosition({ 400.f,450.f });
        m_zonePrev = sf::Rect<float>({ 410.f,200.f }, { PANEL_WIDTH * 0.45f,H });

        m_panelNext.setScale({ 0.45f,0.45f });
        m_panelNext.setPosition({ 600.f,450.f });
        m_zoneNext = sf::Rect<float>({ 610.f,200.f }, { PANEL_WIDTH * 0.45f,H });
    }
    else // scene 3
    {
        m_hasPrev = true;
        m_groundY = 700.f;
        m_player.setScale(0.85f);

        m_panelPrev.setScale({ 0.35f,0.35f });
        m_panelPrev.setPosition({ 250.f,450.f });
        m_zonePrev = sf::Rect<float>({ 270.f,200.f }, { PANEL_WIDTH * 0.25f,H });
    }

    m_pressE.setString("");
    m_pressE.setFillColor(sf::Color(255, 255, 255, 0));
}

void Level::update(float dt)
{
    m_player.update(dt);

    sf::Vector2f pos = m_player.getPosition();
    if (pos.y > m_groundY) pos.y = m_groundY;
    m_player.setPosition(pos);

    sf::Rect<float> playerRect(pos, { 32.f,48.f });

    bool show = false;

    if (m_hasNext && aabbOverlap(playerRect, m_zoneNext))
    {
        show = true;
        m_pressE.setString("Press E");
        m_pressE.setPosition(m_zoneNext.position + sf::Vector2f{ 0.f,220.f });
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::E))
            if (onNextStation) onNextStation();
    }
    else if (m_hasPrev && aabbOverlap(playerRect, m_zonePrev))
    {
        show = true;
        m_pressE.setString("Press E");
        m_pressE.setPosition(m_zonePrev.position + sf::Vector2f{ 0.f,220.f });
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::E))
            if (onPrevStation) onPrevStation();
    }

    m_pressE.setFillColor(sf::Color(255, 255, 255, show ? 255 : 0));
}

void Level::draw(sf::RenderWindow& window)
{
    if (m_sceneTexPtr)
    {
        sf::Sprite bg(*m_sceneTexPtr);
        bg.setPosition({ 0.f,0.f });
        window.draw(bg);
    }

    if (m_hasPrev) window.draw(m_panelPrev);
    if (m_hasNext) window.draw(m_panelNext);

    if (m_pressE.getFillColor().a > 0)
        window.draw(m_pressE);

    m_player.draw(window);
}
