#include "Level.h"
#include <SFML/Window/Keyboard.hpp>

static constexpr float PANEL_WIDTH = 150.f;

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

    m_pressE.setFillColor(sf::Color(255, 255, 255, 0));
}

void Level::setScene(int id, const sf::Texture& tex)
{
    m_sceneTexPtr = &tex;

    // reset NPC
    m_npcs.clear();

    const float h = 2000.f;

    m_hasPrev = false;
    m_hasNext = false;
    m_panelNext.setScale({ 1.f,1.f });
    m_panelPrev.setScale({ 1.f,1.f });

    if (id == 1)
    {
        m_hasNext = true;

        m_panelNext.setScale({ 0.45f,0.45f });
        m_panelNext.setPosition({ 600.f,400.f });
        m_zoneNext = { {610.f,140.f}, {PANEL_WIDTH * 0.45f,h} };

        // npc
        m_npcs.emplace_back("assets/sprites/player.png", sf::Vector2f{ 150,520 });
        m_npcs.back().setScale(1.0f);

        m_groundY = 540.f;

        // <<< border clamp scene 1 >>>
        m_minX = 0.f;
        m_maxX = 850.f;     // ← tu changes ces 2 chiffres plus tard
    }
    else if (id == 2)
    {
        m_hasPrev = true;
        m_hasNext = true;

        m_panelPrev.setScale({ 0.45f,0.45f });
        m_panelPrev.setPosition({ 400.f,450.f });
        m_zonePrev = { {410.f,200.f}, {PANEL_WIDTH * 0.45f,h} };

        m_panelNext.setScale({ 0.45f,0.45f });
        m_panelNext.setPosition({ 600.f,450.f });
        m_zoneNext = { {610.f,200.f}, {PANEL_WIDTH * 0.45f,h} };

        m_npcs.emplace_back("assets/sprites/player.png", sf::Vector2f{ 800,500 });
        m_npcs.back().setScale(1.0f);

        m_groundY = 600.f;

        // <<< border clamp scene 2 >>>
        m_minX = 0.f;
        m_maxX = 1700.f;
    }
    else // 3
    {
        m_hasPrev = true;

        m_panelPrev.setScale({ 0.35f,0.35f });
        m_panelPrev.setPosition({ 250.f,450.f });
        m_zonePrev = { {270.f,200.f}, {PANEL_WIDTH * 0.25f,h} };

        m_npcs.emplace_back("assets/sprites/player.png", sf::Vector2f{ 480,500 });
        m_npcs.back().setScale(0.70f);

        m_groundY = 700.f;

        m_player.setScale(0.85f);

        // <<< border clamp scene 3 >>>
        m_minX = 0.f;
        m_maxX = 1000.f;
    }



    m_pressE.setString("");
    m_pressE.setFillColor(sf::Color(255, 255, 255, 0));
}

void Level::update(float dt)
{
    m_player.update(dt);
    // clamp horizontal borders
    if (m_player.getPosition().x < m_minX)
        m_player.setPosition({ m_minX, m_player.getPosition().y });

    if (m_player.getPosition().x > m_maxX)
        m_player.setPosition({ m_maxX, m_player.getPosition().y });

    if (m_player.getPosition().y > m_groundY)
        m_player.setPosition({ m_player.getPosition().x,m_groundY });

    for (auto& n : m_npcs)
        n.update(dt);

    sf::Rect<float> playerRect(m_player.getPosition(), { 32.f,48.f });

    bool show = false;

    if (m_hasNext && aabbOverlap(playerRect, m_zoneNext))
    {
        show = true;
        m_pressE.setString("Press E");
        m_pressE.setPosition(m_zoneNext.position + sf::Vector2f{ 0,220 });

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::E))
            if (onNextStation) onNextStation();
    }
    else if (m_hasPrev && aabbOverlap(playerRect, m_zonePrev))
    {
        show = true;
        m_pressE.setString("Press E");
        m_pressE.setPosition(m_zonePrev.position + sf::Vector2f{ 0,220 });

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

    for (auto& n : m_npcs)
        n.draw(window);

    if (m_pressE.getFillColor().a > 0)
        window.draw(m_pressE);

    m_player.draw(window);
}
