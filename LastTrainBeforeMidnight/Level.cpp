#include "Level.h"
#include <SFML/Window/Keyboard.hpp>
#include <fstream>
#include <sstream>
#include <cstdint>

static constexpr float PANEL_WIDTH = 150.f;
static constexpr float HINT_FADE_SPEED = 30.f;
static constexpr float NPC_ICON_Y_OFF = 0.f;
static constexpr float PANEL_ICON_Y_OFF = 0.f;
static constexpr float TRAIN_ICON_Y_OFF = -40.f;

// ----------------------------------------
static inline std::vector<std::string> loadSceneDialog(int sceneId)
{
    std::vector<std::string> r;
    std::ifstream f("assets/dialogue/scene" + std::to_string(sceneId) + ".dlg");
    if (!f.is_open()) return r;
    std::string line;
    while (std::getline(f, line)) if (!line.empty()) r.push_back(line);
    return r;
}

static inline void centerDialog(sf::Text& t)
{
    auto b = t.getLocalBounds();
    t.setOrigin({ b.size.x * 0.5f, 0.f });
    t.setPosition({ 960.f, 930.f });
}

// ----------------------------------------
bool Level::aabbOverlap(const sf::Rect<float>& a, const sf::Rect<float>& b)
{
    return (a.position.x < b.position.x + b.size.x) &&
        (a.position.x + a.size.x > b.position.x) &&
        (a.position.y < b.position.y + b.size.y) &&
        (a.position.y + a.size.y > b.position.y);
}

// ----------------------------------------
Level::Level()
    : m_panelTex("assets/sprites/panel.png")
    , m_panelNext(m_panelTex)
    , m_panelPrev(m_panelTex)
    , m_keyTex("assets/sprites/keyE.png")
    , m_keySpritePanel(m_keyTex)
    , m_keySpriteNPC(m_keyTex)
    , m_keySpriteTrain(m_keyTex)
    , m_font("assets/fonts/arial.ttf")
    , m_dialogue(m_font)
{
    // icônes invisibles
    m_keySpritePanel.setColor(sf::Color(255, 255, 255, 0));
    m_keySpriteNPC.setColor(sf::Color(255, 255, 255, 0));
    m_keySpriteTrain.setColor(sf::Color(255, 255, 255, 0));

    // fade noir d'entrée
    m_fader.startFadeIn();
}

// ----------------------------------------
void Level::requestSceneChange(int newId)
{
    if (!m_sceneChangePending)
    {
        m_sceneChangePending = true;
        m_nextSceneId = newId;
        m_fader.startFadeOut();
    }
}

// ----------------------------------------
void Level::setScene(int id, const sf::Texture& tex)
{
    m_sceneId = id;
    m_sceneTexPtr = &tex;

    // reset dialogue / npcs / hints
    closeDialogue();
    m_npcs.clear();

    m_panelHintAlpha = 0;
    m_npcHintAlpha = 0;
    m_trainHintAlpha = 0;
    m_keySpritePanel.setColor(sf::Color(255, 255, 255, 0));
    m_keySpriteNPC.setColor(sf::Color(255, 255, 255, 0));
    m_keySpriteTrain.setColor(sf::Color(255, 255, 255, 0));

    // reset joueur
    m_player.setScale(1.f);

    m_hasPrev = false;
    m_hasNext = false;
    m_hasTrain = false;

    const float h = 2000.f;

    if (id == 1)
    {
        // panneaux
        m_hasNext = true;
        m_panelNext.setScale({ 3,3 });
        m_panelNext.setPosition({ 550,400 });
        m_zoneNext = { {450,140},{PANEL_WIDTH, h} };

        // train (X=800)
        m_hasTrain = true;
        m_zoneTrain = { {800, 140}, { 60, h } };

        // NPC
        m_npcs.emplace_back("assets/sprites/player.png", sf::Vector2f{ 150,520 });
        m_npcs.back().setScale(1.f);

        // limites
        m_groundY = 540;
        m_minX = 0; m_maxX = 850;
    }
    else if (id == 2)
    {
        m_hasPrev = true;
        m_hasNext = true;

        m_panelPrev.setScale({ 3,3 });
        m_panelPrev.setPosition({ 400,450 });
        m_zonePrev = { {310,200},{PANEL_WIDTH, h} };

        m_panelNext.setScale({ 3,3 });
        m_panelNext.setPosition({ 600,450 });
        m_zoneNext = { {510,200},{PANEL_WIDTH, h} };

        // train (X=1450)
        m_hasTrain = true;
        m_zoneTrain = { {1450, 140}, { 60, h } };

        m_npcs.emplace_back("assets/sprites/player.png", sf::Vector2f{ 900,500 });
        m_npcs.back().setScale(1.f);

        m_groundY = 600;
        m_minX = 0; m_maxX = 1700;
    }
    else if (id == 3)
    {
        m_hasPrev = true;

        m_panelPrev.setScale({ 2.5f,2.5f });
        m_panelPrev.setPosition({ 250,450 });
        m_zonePrev = { {170,200},{PANEL_WIDTH, h} };

        // train (X=1000)
        m_hasTrain = true;
        m_zoneTrain = { {1000, 140}, { 60, h } };

        m_npcs.emplace_back("assets/sprites/player.png", sf::Vector2f{ 480,500 });
        m_npcs.back().setScale(0.7f);

        m_groundY = 700;
        m_player.setScale(0.85f);
        m_minX = 0; m_maxX = 1000;
    }
    else // 99 = END
    {
        // pas de panneaux, pas de train, pas de NPC
        m_hasPrev = m_hasNext = m_hasTrain = false;
        m_npcs.clear();
        m_groundY = 1080.f;
        m_minX = 0; m_maxX = 1920;

        // on peut laisser le player mais il ne gêne pas (en bas hors-écran)
        m_player.setPosition({ 960.f, m_groundY });
    }
}

// ----------------------------------------
void Level::update(float dt)
{
    // fade
    m_fader.update(dt);

    // si on attend un switch et qu’on est noir -> demander à Game la nouvelle texture
    if (m_sceneChangePending && m_fader.isBlack())
    {
        if (onRequestSceneTexture)
            onRequestSceneTexture(m_nextSceneId);

        m_sceneChangePending = false;
        m_fader.startFadeIn();
    }

    // inputs (edges)
    bool space = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space);
    bool eDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::E);
    bool spaceEdge = (space && !m_spaceWasDown);
    bool eEdge = (eDown && !m_eWasDown);
    m_spaceWasDown = space;
    m_eWasDown = eDown;

    // gameplay (hors dialogue et hors écran de fin)
    if (!m_dialogue.active() && m_sceneId != 99)
    {
        m_player.update(dt);
        if (m_player.getPosition().x < m_minX) m_player.setPosition({ m_minX, m_player.getPosition().y });
        if (m_player.getPosition().x > m_maxX) m_player.setPosition({ m_maxX, m_player.getPosition().y });
        if (m_player.getPosition().y > m_groundY) m_player.setPosition({ m_player.getPosition().x, m_groundY });
    }

    for (auto& n : m_npcs) n.update(dt);

    sf::Rect<float> playerRect(m_player.getPosition(), { 32,48 });

    // PANNEAUX
    bool showPanel = false, onNext = false, onPrev = false;
    if (m_hasNext && aabbOverlap(playerRect, m_zoneNext)) { showPanel = true; onNext = true; }
    else if (m_hasPrev && aabbOverlap(playerRect, m_zonePrev)) { showPanel = true; onPrev = true; }

    if (!m_dialogue.active() && eEdge && m_sceneId != 99)
    {
        if (onNext) requestSceneChange(m_sceneId + 1);
        else if (onPrev) requestSceneChange(m_sceneId - 1);
    }

    // fade panneau
    {
        float k = HINT_FADE_SPEED * dt;
        int target = showPanel ? 255 : 0;
        m_panelHintAlpha = (int)(m_panelHintAlpha + (target - m_panelHintAlpha) * k);
        if (m_panelHintAlpha < 0) m_panelHintAlpha = 0;
        if (m_panelHintAlpha > 255) m_panelHintAlpha = 255;
    }

    // TRAIN
    bool showTrain = false;
    if (m_hasTrain && aabbOverlap(playerRect, m_zoneTrain))
        showTrain = true;

    if (!m_dialogue.active() && eEdge && m_sceneId != 99 && showTrain)
    {
        // logique : si station gagnante -> END(99) sinon retour 1
        int dest = (m_sceneId == m_winningStation) ? 99 : 1;
        requestSceneChange(dest);
    }

    {
        float k = HINT_FADE_SPEED * dt;
        int target = showTrain ? 255 : 0;
        m_trainHintAlpha = (int)(m_trainHintAlpha + (target - m_trainHintAlpha) * k);
        if (m_trainHintAlpha < 0) m_trainHintAlpha = 0;
        if (m_trainHintAlpha > 255) m_trainHintAlpha = 255;
    }

    // NPC (hover colonne)
    int hoverNpc = -1;
    for (int i = 0; i < (int)m_npcs.size(); ++i)
    {
        auto np = m_npcs[i].getPosition();
        sf::Rect<float> npcZone({ np.x - 20, 0 }, { 40, 2000 });
        if (aabbOverlap(playerRect, npcZone)) { hoverNpc = i; break; }
    }

    if (!m_dialogue.active() && m_sceneId != 99)
    {
        float k = HINT_FADE_SPEED * dt;
        if (hoverNpc >= 0)
        {
            m_npcHintAlpha = (int)(m_npcHintAlpha + (255 - m_npcHintAlpha) * k);
            if (m_npcHintAlpha > 255) m_npcHintAlpha = 255;

            if (eEdge) beginDialogueForNpc(hoverNpc);
        }
        else
        {
            m_npcHintAlpha = (int)(m_npcHintAlpha + (0 - m_npcHintAlpha) * k);
            if (m_npcHintAlpha < 0) m_npcHintAlpha = 0;
        }
    }
    else
    {
        float k = HINT_FADE_SPEED * dt;
        m_npcHintAlpha = (int)(m_npcHintAlpha + (0 - m_npcHintAlpha) * k);
        if (m_npcHintAlpha < 0) m_npcHintAlpha = 0;

        if (spaceEdge || eEdge) advanceDialogue();
    }

    // positions icônes
    if (m_panelHintAlpha > 0)
    {
        auto b = onNext ? m_panelNext.getGlobalBounds() : m_panelPrev.getGlobalBounds();
        m_keySpritePanel.setPosition({ b.position.x + b.size.x * 0.5f, b.position.y + PANEL_ICON_Y_OFF });
    }

    if (m_trainHintAlpha > 0 && showTrain)
    {
        auto bx = m_zoneTrain.position.x + m_zoneTrain.size.x * 0.5f;
        m_keySpriteTrain.setPosition({ bx, m_zoneTrain.position.y + TRAIN_ICON_Y_OFF });
    }
}

// ----------------------------------------
void Level::draw(sf::RenderWindow& window)
{
    // fond
    if (m_sceneTexPtr)
    {
        sf::Sprite bg(*m_sceneTexPtr);
        bg.setPosition({ 0,0 });
        window.draw(bg);
    }

    // panneaux
    if (m_hasPrev) window.draw(m_panelPrev);
    if (m_hasNext) window.draw(m_panelNext);

    // NPC
    for (auto& n : m_npcs) n.draw(window);

    // icones
    if (m_panelHintAlpha > 0)
    {
        auto c = m_keySpritePanel.getColor(); c.a = (std::uint8_t)m_panelHintAlpha;
        m_keySpritePanel.setColor(c);
        window.draw(m_keySpritePanel);
    }
    if (m_trainHintAlpha > 0)
    {
        auto c = m_keySpriteTrain.getColor(); c.a = (std::uint8_t)m_trainHintAlpha;
        m_keySpriteTrain.setColor(c);
        window.draw(m_keySpriteTrain);
    }
    if (m_npcHintAlpha > 0)
    {
        auto c = m_keySpriteNPC.getColor(); c.a = (std::uint8_t)m_npcHintAlpha;
        m_keySpriteNPC.setColor(c);
        window.draw(m_keySpriteNPC);
    }

    // player (pas sur l’écran de fin)
    if (m_sceneId != 99)
        m_player.draw(window);

    // dialogue (sous-titres)
    m_dialogue.draw(window);

    // fader en dernier
    m_fader.draw(window);
}

// ----------------------------------------
// Dialogue
void Level::beginDialogueForNpc(int npcIndex)
{
    m_activeNpcIndex = npcIndex;

    // pousse légèrement le joueur à côté pour ne pas cacher le NPC
    auto npcPos = m_npcs[npcIndex].getPosition();
    if (m_player.getPosition().x < npcPos.x)
        m_player.setPosition({ npcPos.x - 150.f, m_groundY });
    else
        m_player.setPosition({ npcPos.x + 150.f, m_groundY });

    // charge lignes
    auto lines = loadSceneDialog(m_sceneId);
    if (lines.empty()) { closeDialogue(); return; }

    m_dialogue.startWithLines(lines);
}

void Level::advanceDialogue()
{
    if (!m_dialogue.active()) return;
    m_dialogue.advance();
    if (!m_dialogue.active())
        closeDialogue();
}

void Level::closeDialogue()
{
    m_dialogue.end();
    m_activeNpcIndex = -1;
}
