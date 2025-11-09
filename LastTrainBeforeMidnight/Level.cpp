#include "Level.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Audio.hpp>
#include <fstream>
#include <sstream>
#include <cstdint>

// Accès musique globale + signal volume pour Game
extern sf::Music* gMusic;
extern float g_pendingSceneVolume; // <-- Game écoute ce flag

static constexpr float PANEL_WIDTH = 150.f;

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
    , m_hintIcons(m_keySpritePanel, m_keySpriteNPC, m_keySpriteTrain)
    , m_npcInteraction(m_dialogue)
{
    // réglages visuels init
    m_hintIcons.setFadeSpeed(30.f);
    m_hintIcons.setOffsets(/*panel*/ 0.f, /*npc*/ -40.f, /*train*/ -40.f);

    // fade noir d'entrée
    m_fader.startFadeIn();

    // audio state (si tu utilises le duck de dialogue ici, garde-le)
    m_dialoguePrevActive = false;
    m_targetPitch = m_currentPitch = 1.f;
    m_targetVolume = m_currentVolume = 40.f; // mets 40 si ta base jeu est 40, sinon adapte
}

// ----------------------------------------
void Level::requestSceneChange(int newId)
{
    if (!m_sceneChangePending)
    {
        m_sceneChangePending = true;
        m_nextSceneId = newId;

        // AUDIO SCENE SWITCH : descendre la musique à 5% pendant le fade-out + noir
        g_pendingSceneVolume = 5.f;

        m_fader.startFadeOut();
    }
}

// ----------------------------------------
void Level::setScene(int id, const sf::Texture& tex)
{
    m_sceneId = id;
    m_sceneTexPtr = &tex;

    // reset dialogue / npcs
    m_npcInteraction.handleAdvance(false, false); // force close if open (no-op if closed)
    while (m_dialogue.active()) m_dialogue.end();

    m_npcs.clear();

    // reset joueur
    m_player.setScale(1.f);

    m_hasPrev = false;
    m_hasNext = false;
    m_hasTrain = false;

    const float h = 2000.f;

    if (id == 1)
    {
        m_hasNext = true;
        m_panelNext.setScale({ 3,3 });
        m_panelNext.setPosition({ 550,400 });
        m_zoneNext = { {450,140},{PANEL_WIDTH, h} };

        m_hasTrain = true;
        m_zoneTrain = { {800, 140}, { 60, h } };

        m_npcs.emplace_back("assets/sprites/player.png", sf::Vector2f{ 150,520 });
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
        m_zonePrev = { {310,200},{PANEL_WIDTH, h} };

        m_panelNext.setScale({ 3,3 });
        m_panelNext.setPosition({ 600,450 });
        m_zoneNext = { {510,200},{PANEL_WIDTH, h} };

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
        m_hasPrev = m_hasNext = m_hasTrain = false;
        m_npcs.clear();
        m_groundY = 1080.f;
        m_minX = 0; m_maxX = 1920;

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
    m_input.update();
    const bool spaceEdge = m_input.spaceEdge();
    const bool eEdge = m_input.eEdge();

    // gameplay (hors dialogue et hors écran de fin)
    if (!m_dialogue.active() && m_sceneId != 99)
    {
        m_player.update(dt);
        if (m_player.getPosition().x < m_minX) m_player.setPosition({ m_minX, m_player.getPosition().y });
        if (m_player.getPosition().x > m_maxX) m_player.setPosition({ m_maxX, m_player.getPosition().y });
        if (m_player.getPosition().y > m_groundY) m_player.setPosition({ m_player.getPosition().x, m_groundY });
    }

    for (auto& n : m_npcs) n.update(dt);

    // rectangles
    sf::Rect<float> playerRect(m_player.getPosition(), { 32,48 });

    // ---- NPC interaction ----
    const int hoverNpc = (m_sceneId != 99) ? m_npcInteraction.computeHoverNpc(playerRect, m_npcs) : -1;

    if (!m_dialogue.active() && m_sceneId != 99)
    {
        if (hoverNpc >= 0 && eEdge)
        {
            m_npcInteraction.tryBeginDialogue(hoverNpc, m_sceneId, m_groundY, m_player, m_npcs);
        }
    }
    else
    {
        // avancer/fermer
        m_npcInteraction.handleAdvance(spaceEdge, eEdge);
    }

    // ---- Scene Navigation (panneaux + train) ----
    PanelState pstate;
    pstate.hasPrev = m_hasPrev;
    pstate.hasNext = m_hasNext;
    pstate.panelPrev = &m_panelPrev;
    pstate.panelNext = &m_panelNext;
    pstate.zonePrev = m_zonePrev;
    pstate.zoneNext = m_zoneNext;

    TrainState tstate;
    tstate.hasTrain = m_hasTrain;
    tstate.zoneTrain = m_zoneTrain;

    bool showPanel = false, onPrev = false, onNext = false, showTrain = false;
    m_navigation.compute(playerRect, pstate, tstate, showPanel, onPrev, onNext, showTrain);

    if (!m_dialogue.active() && m_sceneId != 99)
    {
        const int decided = m_navigation.decideSceneChange(eEdge, onPrev, onNext, showTrain, m_sceneId, m_winningStation);
        if (decided != -1) requestSceneChange(decided);
    }

    // ---- Hint icons : anchors + update ----
    if (showPanel)
    {
        const auto& sprite = onNext ? m_panelNext : m_panelPrev;
        auto b = sprite.getGlobalBounds();
        m_hintIcons.setPanelAnchor({ b.position.x + b.size.x * 0.5f, b.position.y });
    }

    if (showTrain)
    {
        const float bx = m_zoneTrain.position.x + m_zoneTrain.size.x * 0.5f;
        m_hintIcons.setTrainAnchor({ bx, m_zoneTrain.position.y });
    }

    if (hoverNpc >= 0)
    {
        m_hintIcons.setNpcAnchor(m_npcs[hoverNpc].getPosition());
    }

    m_hintIcons.update(dt, showPanel, (hoverNpc >= 0) && !m_dialogue.active(), showTrain);

    // ---- AUDIO : remonter à 30% quand le fade-in visuel du Level est terminé ----
    {
        static bool s_prevClear = false;
        const bool clearNow = (m_fader.done() && !m_fader.isBlack());
        if (clearNow && !s_prevClear)
        {
            // Fin du fade-in -> Game remontera à 30%
            g_pendingSceneVolume = 30.f;
        }
        s_prevClear = clearNow;
    }

    // (si tu as un duck volume pendant dialogue ici, garde ton code.
    //  Il se combinera avec la cible globale via le lissage de Game.
    //  Conseil: limite-toi à setPitch ici, et laisse Game gérer le volume global.)
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

    // player (pas sur l’écran de fin)
    if (m_sceneId != 99)
        m_player.draw(window);

    // icônes "E"
    m_hintIcons.draw(window);

    // dialogue (sous-titres)
    m_dialogue.draw(window);

    // fader en dernier
    m_fader.draw(window);
}
