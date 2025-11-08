#include "Level.h"
#include <SFML/Window/Keyboard.hpp>
#include <fstream>
#include <sstream>
#include <cstdint>

static constexpr float PANEL_WIDTH = 150.f;
static constexpr float HINT_FADE_SPEED = 30.f;
static constexpr float KEY_SCALE_PANEL = 1.7f;
static constexpr float KEY_SCALE_NPC = 1.2f;
static constexpr float NPC_ICON_Y_OFF = -40.f;
static constexpr float PANEL_ICON_Y_OFF = -20.f;

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
    , m_dialogue(m_font)
    , m_keyTex("assets/sprites/keyE.png")
    , m_keySpritePanel(m_keyTex)
    , m_keySpriteNPC(m_keyTex)
{
    m_keySpritePanel.setScale({ KEY_SCALE_PANEL,KEY_SCALE_PANEL });
    m_keySpriteNPC.setScale({ KEY_SCALE_NPC,KEY_SCALE_NPC });

    m_keySpritePanel.setColor(sf::Color(255, 255, 255, 0));
    m_keySpriteNPC.setColor(sf::Color(255, 255, 255, 0));
}

void Level::setScene(int id, const sf::Texture& tex)
{
    m_sceneId = id;
    m_sceneTexPtr = &tex;

    m_dialogue.end();
    m_npcs.clear();

    const float h = 2000.f;

    m_hasPrev = false;
    m_hasNext = false;

    m_panelNext.setScale({ 1,1 });
    m_panelPrev.setScale({ 1,1 });

    m_player.setScale(1.f);

    m_panelHintAlpha = 0;
    m_npcHintAlpha = 0;

    if (id == 1)
    {
        m_hasNext = true;
        m_panelNext.setScale({ 3,3 });
        m_panelNext.setPosition({ 550,400 });
        m_zoneNext = { {450,140},{PANEL_WIDTH,h} };

        m_npcs.emplace_back("assets/sprites/player.png", sf::Vector2f{ 150.f,520.f });
        m_npcs.back().setScale(1.f);

        m_groundY = 540;
        m_minX = 0; m_maxX = 850;
    }
    else if (id == 2)
    {
        m_hasPrev = true;
        m_hasNext = true;
        m_panelPrev.setScale({ 3,3 });
        m_panelPrev.setPosition({ 400,450 });
        m_zonePrev = { {310,200},{PANEL_WIDTH,h} };
        m_panelNext.setScale({ 3,3 });
        m_panelNext.setPosition({ 600,450 });
        m_zoneNext = { {510,200},{PANEL_WIDTH,h} };

        m_npcs.emplace_back("assets/sprites/player.png", sf::Vector2f{ 900.f,500.f });
        m_npcs.back().setScale(1.f);

        m_groundY = 600;
        m_minX = 0; m_maxX = 1700;
    }
    else
    {
        m_hasPrev = true;
        m_panelPrev.setScale({ 2.5f,2.5f });
        m_panelPrev.setPosition({ 250,450 });
        m_zonePrev = { {170,200},{PANEL_WIDTH,h} };

        m_npcs.emplace_back("assets/sprites/player.png", sf::Vector2f{ 480.f,500.f });
        m_npcs.back().setScale(0.7f);

        m_groundY = 700;
        m_player.setScale(0.85f);

        m_minX = 0; m_maxX = 1000;
    }
}

void Level::update(float dt)
{
    bool spaceDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space);
    bool eDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::E);
    bool spaceEdge = (spaceDown && !m_spaceWasDown);
    bool eEdge = (eDown && !m_eWasDown);
    m_spaceWasDown = spaceDown;
    m_eWasDown = eDown;

    if (!m_dialogue.active())
    {
        m_player.update(dt);
        if (m_player.getPosition().x < m_minX) m_player.setPosition({ m_minX,m_player.getPosition().y });
        if (m_player.getPosition().x > m_maxX) m_player.setPosition({ m_maxX,m_player.getPosition().y });
        if (m_player.getPosition().y > m_groundY) m_player.setPosition({ m_player.getPosition().x,m_groundY });
    }

    for (auto& n : m_npcs) n.update(dt);

    sf::Rect<float> playerRect(m_player.getPosition(), { 32,48 });

    bool showPanel = false; bool onNext = false; bool onPrev = false;
    if (m_hasNext && aabbOverlap(playerRect, m_zoneNext)) { showPanel = true; onNext = true; }
    else if (m_hasPrev && aabbOverlap(playerRect, m_zonePrev)) { showPanel = true; onPrev = true; }

    if (!m_dialogue.active() && eEdge)
    {
        if (onNext && onNextStation) onNextStation();
        else if (onPrev && onPrevStation) onPrevStation();
    }

    {
        float k = HINT_FADE_SPEED * dt;
        int target = showPanel ? 255 : 0;
        m_panelHintAlpha = (int)(m_panelHintAlpha + (target - m_panelHintAlpha) * k);
        if (m_panelHintAlpha < 0)m_panelHintAlpha = 0;
        if (m_panelHintAlpha > 255)m_panelHintAlpha = 255;
    }

    int hoverNpc = -1;
    for (int i = 0;i < (int)m_npcs.size();++i)
    {
        auto np = m_npcs[i].getPosition();
        sf::Rect<float> npcZone({ np.x - 20,0 }, { 40,2000 });
        if (aabbOverlap(playerRect, npcZone)) { hoverNpc = i;break; }
    }

    if (!m_dialogue.active())
    {
        float k = HINT_FADE_SPEED * dt;
        if (hoverNpc >= 0)
        {
            m_npcHintAlpha = (int)(m_npcHintAlpha + (255 - m_npcHintAlpha) * k);
            if (m_npcHintAlpha > 255)m_npcHintAlpha = 255;
            if (eEdge)
                m_dialogue.startScene(m_sceneId, m_player.getPosition(), m_npcs[hoverNpc].getPosition());
        }
        else
        {
            m_npcHintAlpha = (int)(m_npcHintAlpha + (0 - m_npcHintAlpha) * k);
            if (m_npcHintAlpha < 0)m_npcHintAlpha = 0;
        }
    }
    else
    {
        float k = HINT_FADE_SPEED * dt;
        m_npcHintAlpha = (int)(m_npcHintAlpha + (0 - m_npcHintAlpha) * k);
        if (m_npcHintAlpha < 0)m_npcHintAlpha = 0;

        if (spaceEdge || eEdge) m_dialogue.advance();
    }

    if (m_panelHintAlpha > 0)
    {
        auto b = onNext ? m_panelNext.getGlobalBounds() : m_panelPrev.getGlobalBounds();
        m_keySpritePanel.setPosition({ b.position.x + b.size.x * 0.5f,b.position.y + PANEL_ICON_Y_OFF });
    }

    if (m_npcHintAlpha > 0 && hoverNpc >= 0)
    {
        auto np = m_npcs[hoverNpc].getPosition();
        m_keySpriteNPC.setPosition({ np.x,np.y + NPC_ICON_Y_OFF });
    }
}

void Level::draw(sf::RenderWindow& window)
{
    if (m_sceneTexPtr)
    {
        sf::Sprite bg(*m_sceneTexPtr);
        bg.setPosition({ 0,0 });
        window.draw(bg);
    }

    if (m_hasPrev)window.draw(m_panelPrev);
    if (m_hasNext)window.draw(m_panelNext);

    for (auto& n : m_npcs) n.draw(window);

    if (m_panelHintAlpha > 0)
    {
        auto c = m_keySpritePanel.getColor(); c.a = (uint8_t)m_panelHintAlpha;
        m_keySpritePanel.setColor(c);
        window.draw(m_keySpritePanel);
    }

    if (m_npcHintAlpha > 0)
    {
        auto c = m_keySpriteNPC.getColor(); c.a = (uint8_t)m_npcHintAlpha;
        m_keySpriteNPC.setColor(c);
        window.draw(m_keySpriteNPC);
    }

    m_player.draw(window);

    m_dialogue.draw(window);
}
