#include "Level.h"
#include <SFML/Window/Keyboard.hpp>
#include <fstream>
#include <sstream>

static constexpr float PANEL_WIDTH = 150.f;

// ------------------------------------------------------------------
// util: overlap Rect<float> (SFML3: position + size)
// ------------------------------------------------------------------
bool Level::aabbOverlap(const sf::Rect<float>& a, const sf::Rect<float>& b)
{
    return (a.position.x < b.position.x + b.size.x) &&
        (a.position.x + a.size.x > b.position.x) &&
        (a.position.y < b.position.y + b.size.y) &&
        (a.position.y + a.size.y > b.position.y);
}

// ------------------------------------------------------------------
// ctor
// ------------------------------------------------------------------
Level::Level()
    : m_panelTex("assets/sprites/panel.png")
    , m_panelNext(m_panelTex)
    , m_panelPrev(m_panelTex)
    , m_font("assets/fonts/arial.ttf")
    , m_pressE(m_font, "", 28)
    , m_pressE_NPC(m_font, "", 24)
    , m_dialogText(m_font, "", 32)
{
    m_pressE.setFillColor(sf::Color(255, 255, 255, 0));
    m_pressE_NPC.setFillColor(sf::Color(255, 255, 255, 0));

    // sous-titre centré en bas (option 1)
    m_dialogText.setFillColor(sf::Color(255, 255, 255, 255));
    // position Y fixe (bas de l’écran)
    m_dialogText.setPosition(sf::Vector2f(960.f, 930.f)); // x centré (origine modifiée plus bas)
    m_dialogText.setOrigin(sf::Vector2f(0.f, 0.f));       // on remettra l'origine à chaque setString pour centrer
}

// ------------------------------------------------------------------
// helper: charge assets/dialogue/scene{sceneId}.dlg (1 ligne = 1 phrase)
// ------------------------------------------------------------------
static std::vector<std::string> loadSceneDialog(int sceneId)
{
    std::vector<std::string> result;

    std::string path = "assets/dialogue/scene" + std::to_string(sceneId) + ".dlg";
    std::ifstream file(path);
    if (!file.is_open())
        return result;

    std::string line;
    while (std::getline(file, line))
    {
        if (!line.empty())
            result.push_back(line);
    }
    return result;
}

// ------------------------------------------------------------------
// setScene
// ------------------------------------------------------------------
void Level::setScene(int id, const sf::Texture& tex)
{
    m_sceneId = id;
    m_sceneTexPtr = &tex;

    closeDialogue();
    m_npcs.clear();

    const float h = 2000.f;

    m_hasPrev = false;
    m_hasNext = false;
    m_panelNext.setScale({ 1.f,1.f });
    m_panelPrev.setScale({ 1.f,1.f });

    // <<< FIX scale >>>
    m_player.setScale(1.f);  // reset scale par défaut

    // nettoyer hints
    m_pressE.setString("");
    m_pressE.setFillColor(sf::Color(255, 255, 255, 0));
    m_pressE_NPC.setString("");
    m_pressE_NPC.setFillColor(sf::Color(255, 255, 255, 0));

    if (id == 1)
    {
        m_hasNext = true;

        m_panelNext.setScale({ 3.0f,3.0f });
        m_panelNext.setPosition({ 550.f,400.f });
        m_zoneNext = { {450.f,140.f}, {PANEL_WIDTH ,h} };

        // NPC scène 1
        m_npcs.emplace_back("assets/sprites/player.png", sf::Vector2f{ 150,520 });
        m_npcs.back().setScale(1.0f);

        m_groundY = 540.f;

        // bordures S1 (tes valeurs)
        m_minX = 0.f;
        m_maxX = 850.f;
    }
    else if (id == 2)
    {
        m_hasPrev = true;
        m_hasNext = true;

        m_panelPrev.setScale({ 3.0f,3.0f });
        m_panelPrev.setPosition({ 400.f,450.f });
        m_zonePrev = { {310.f,200.f}, {PANEL_WIDTH ,h} };

        m_panelNext.setScale({ 3.0f,3.0f });
        m_panelNext.setPosition({ 600.f,450.f });
        m_zoneNext = { {510.f,200.f}, {PANEL_WIDTH  ,h} };

        // NPC scène 2
        m_npcs.emplace_back("assets/sprites/player.png", sf::Vector2f{ 900,500 });
        m_npcs.back().setScale(1.0f);

        m_groundY = 600.f;

        // bordures S2 (tes valeurs)
        m_minX = 0.f;
        m_maxX = 1700.f;
    }
    else // 3
    {
        m_hasPrev = true;

        m_panelPrev.setScale({ 2.5f,2.5f });
        m_panelPrev.setPosition({ 250.f,450.f });
        m_zonePrev = { {170.f,200.f}, {PANEL_WIDTH,h} };

        // NPC scène 3
        m_npcs.emplace_back("assets/sprites/player.png", sf::Vector2f{ 480,500 });
        m_npcs.back().setScale(0.70f);

        m_groundY = 700.f;

        // player scale scène 3 (comme tu l’as demandé)
        m_player.setScale(0.85f);

        // bordures S3 (tes valeurs)
        m_minX = 0.f;
        m_maxX = 1000.f;
    }
}

// ------------------------------------------------------------------
// update
// ------------------------------------------------------------------
void Level::update(float dt)
{
    // détection SPACE (edge)
    bool spaceDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space);
    bool spaceEdge = (spaceDown && !m_spaceWasDown);
    m_spaceWasDown = spaceDown;

    if (!m_inDialogue)
    {
        // joueur libre
        m_player.update(dt);

        // clamp horizontal
        if (m_player.getPosition().x < m_minX)
            m_player.setPosition({ m_minX, m_player.getPosition().y });

        if (m_player.getPosition().x > m_maxX)
            m_player.setPosition({ m_maxX, m_player.getPosition().y });

        // sol
        if (m_player.getPosition().y > m_groundY)
            m_player.setPosition({ m_player.getPosition().x, m_groundY });

        // idle NPC
        for (auto& n : m_npcs)
            n.update(dt);

        // ---- interaction NPC (Press E) ----
        int hoverNpc = -1;
        {
            sf::Rect<float> playerRect(m_player.getPosition(), sf::Vector2f{ 32.f,48.f });

            for (int i = 0; i < (int)m_npcs.size(); ++i)
            {
                const sf::Vector2f np = m_npcs[i].getPosition();

                // colonne trigger verticale NPC comme panneau (infini)
                sf::Rect<float> npcZone(
                    sf::Vector2f{ np.x - 20.f, 0.f },
                    sf::Vector2f{ 40.f, 2000.f }
                );


                if (aabbOverlap(playerRect, npcZone))
                {
                    hoverNpc = i;
                    break;
                }
            }
        }

        if (hoverNpc >= 0)
        {
            // afficher "Press E" (blanc) au-dessus du NPC
            m_pressE_NPC.setString("Press E");
            m_pressE_NPC.setFillColor(sf::Color(255, 255, 255, 255));
            m_pressE_NPC.setPosition(m_npcs[hoverNpc].getPosition() + sf::Vector2f{ 80.f, -35.f });

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::E))
            {
                beginDialogueForNpc(hoverNpc);
            }
        }
        else
        {
            // cacher hint NPC
            m_pressE_NPC.setString("");
            m_pressE_NPC.setFillColor(sf::Color(255, 255, 255, 0));
        }
    }
    else
    {
        // --- en dialogue : freeze player ---
        // (les panneaux restent actifs plus bas)

        // NPC continuent d'idle
        for (auto& n : m_npcs)
            n.update(dt);

        // avance dialogue
        if (spaceEdge)
            advanceDialogue();
    }

    // ---- Panneaux (actifs même pendant dialogue) ----
    sf::Rect<float> playerRect(m_player.getPosition(), { 32.f,48.f });

    bool showPanelHint = false;

    if (m_hasNext && aabbOverlap(playerRect, m_zoneNext))
    {
        showPanelHint = true;
        m_pressE.setString("Press E");
        m_pressE.setPosition(m_zoneNext.position + sf::Vector2f{ 100,220 });

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::E))
            if (onNextStation) onNextStation();
    }
    else if (m_hasPrev && aabbOverlap(playerRect, m_zonePrev))
    {
        showPanelHint = true;
        m_pressE.setString("Press E");
        m_pressE.setPosition(m_zonePrev.position + sf::Vector2f{ 100,220 });

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::E))
            if (onPrevStation) onPrevStation();
    }

    m_pressE.setFillColor(sf::Color(255, 255, 255, showPanelHint ? 255 : 0));

    // NPC hint : disparaît pendant dialogue (ton choix A)
    if (m_inDialogue)
    {
        m_pressE_NPC.setString("");
        m_pressE_NPC.setFillColor(sf::Color(255, 255, 255, 0));
    }
}

// ------------------------------------------------------------------
// draw
// ------------------------------------------------------------------
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

    // NPC
    for (auto& n : m_npcs)
        n.draw(window);

    // hints
    if (m_pressE.getFillColor().a > 0)
        window.draw(m_pressE);

    if (m_pressE_NPC.getFillColor().a > 0)
        window.draw(m_pressE_NPC);

    // player
    m_player.draw(window);

    // sous-titre (dialogue)
    if (m_inDialogue && !m_dialogLines.empty())
    {
        // center text horizontally (origin = half width)
        // SFML 3: getLocalBounds returns Rect<float> with size
        auto bounds = m_dialogText.getLocalBounds();
        m_dialogText.setOrigin(sf::Vector2f{ bounds.size.x * 0.5f, 0.f });

        window.draw(m_dialogText);
    }
}

// ------------------------------------------------------------------
// dialogue helpers
// ------------------------------------------------------------------
void Level::beginDialogueForNpc(int npcIndex)
{
    m_activeNpcIndex = npcIndex;

    // charge le fichier .dlg de la scène courante
    m_dialogLines = loadSceneDialog(m_sceneId);
    m_dialogIndex = 0;

    if (!m_dialogLines.empty())
    {
        m_inDialogue = true;

        // affiche 1ère ligne
        m_dialogText.setString(m_dialogLines[m_dialogIndex]);
        // --- offset player to not overlap NPC visually ---
        auto npcPos = m_npcs[npcIndex].getPosition();

        // si le player est à gauche du NPC -> pousse le player encore plus à gauche
        if (m_player.getPosition().x < npcPos.x)
            m_player.setPosition({ npcPos.x - 150.f, m_player.getPosition().y });
        else
            m_player.setPosition({ npcPos.x + 150.f, m_player.getPosition().y });


        // on cache le hint NPC pendant le dialogue (choix A)
        m_pressE_NPC.setString("");
        m_pressE_NPC.setFillColor(sf::Color(255, 255, 255, 0));
    }
    else
    {
        // pas de lignes -> pas de dialogue
        closeDialogue();
    }
}

void Level::advanceDialogue()
{
    if (!m_inDialogue) return;
    if (m_dialogLines.empty()) { closeDialogue(); return; }

    if (m_dialogIndex + 1 < m_dialogLines.size())
    {
        ++m_dialogIndex;
        m_dialogText.setString(m_dialogLines[m_dialogIndex]);
    }
    else
    {
        closeDialogue();
    }
}

void Level::closeDialogue()
{
    m_inDialogue = false;
    m_activeNpcIndex = -1;
    m_dialogLines.clear();
    m_dialogIndex = 0;

    // effacer le sous-titre
    m_dialogText.setString("");
}
