#include "Level.h"
#include <SFML/Window/Keyboard.hpp>

static constexpr float PANEL_WIDTH = 150.f;
static constexpr float PANEL_HEIGHT = 120.f;

// AABB pour Rect<float> SFML3 (position + size)
bool Level::aabbOverlap(const sf::Rect<float>& a, const sf::Rect<float>& b)
{
    return (a.position.x < b.position.x + b.size.x) &&
        (a.position.x + a.size.x > b.position.x) &&
        (a.position.y < b.position.y + b.size.y) &&
        (a.position.y + a.size.y > b.position.y);
}

// --------------------------------------------
// ctor
// --------------------------------------------
Level::Level()
    : m_panelTex("assets/sprites/panel.png")
    , m_panelNext(m_panelTex)                // <-- IMPORTANT: SFML 3 exige une texture au ctor
    , m_panelPrev(m_panelTex)                // <-- idem
    , m_font("assets/fonts/arial.ttf")
    , m_pressE(m_font, "", 28)
{
    // Texte invisible par défaut
    m_pressE.setFillColor(sf::Color(255, 255, 255, 0));

    m_sceneTexPtr = nullptr;
    m_hasNext = false;
    m_hasPrev = false;
}

// --------------------------------------------
// appelé par GAME quand il change de scène
// --------------------------------------------
void Level::setScene(int sceneId, const sf::Texture& sceneTexture)
{
    m_sceneTexPtr = &sceneTexture;
    configureForScene(sceneId);
}

// --------------------------------------------
// configuration de chaque scène
// --------------------------------------------
void Level::configureForScene(int sceneId)
{
    // barre verticale "infinie" pour le trigger
    const float h = 2000.f;

    // Remise à zéro systématique pour éviter les effets de scènes précédentes
    m_hasNext = false;
    m_hasPrev = false;
    m_panelNext.setScale(sf::Vector2f{ 1.f, 1.f });
    m_panelPrev.setScale(sf::Vector2f{ 1.f, 1.f });

    if (sceneId == 1)
    {
        m_hasPrev = false;
        m_hasNext = true;

        // Scale spécifique scène 1
        m_panelNext.setScale(sf::Vector2f{ 0.45f, 0.45f });
        m_panelNext.setPosition(sf::Vector2f{ 600.f, 400.f });

        // Trigger colonne (tu as validé ces valeurs)
        m_zoneNext = sf::Rect<float>(
            sf::Vector2f{ 610.f, 140.f },
            sf::Vector2f{ PANEL_WIDTH * 0.45f, h }
        );
    }
    else if (sceneId == 2)
    {
        // <<< *** CONSERVÉ EXACTEMENT COMME TU L'AS DONNÉ *** >>>
        m_hasPrev = true;
        m_hasNext = true;

        m_panelPrev.setScale(sf::Vector2f{ 0.45f, 0.45f });
        m_panelPrev.setPosition(sf::Vector2f{ 400.f, 450.f });
        m_zonePrev = sf::Rect<float>(
            sf::Vector2f{ 410.f, 200.f },
            sf::Vector2f{ PANEL_WIDTH * 0.45f, h }
        );

        m_panelNext.setScale(sf::Vector2f{ 0.45f, 0.45f });
        m_panelNext.setPosition(sf::Vector2f{ 600.f, 450.f });
        m_zoneNext = sf::Rect<float>(
            sf::Vector2f{ 610.f, 200.f },
            sf::Vector2f{ PANEL_WIDTH * 0.45f, h }
        );
    }
    else // sceneId == 3
    {
        m_hasPrev = true;
        m_hasNext = false;

        // Panneau plus petit sur la 3
        m_panelPrev.setScale(sf::Vector2f{ 0.35f, 0.35f });
        m_panelPrev.setPosition(sf::Vector2f{ 250.f, 450.f });

        // Trigger colonne pour revenir
        m_zonePrev = sf::Rect<float>(
            sf::Vector2f{ 270.f, 200.f },
            sf::Vector2f{ PANEL_WIDTH * 0.25f, h }
        );
    }

    // Reset du texte
    m_pressE.setString("");
    m_pressE.setFillColor(sf::Color(255, 255, 255, 0));
}

// --------------------------------------------
// UPDATE
// --------------------------------------------
void Level::update(float dt)
{
    m_player.update(dt);

    sf::Rect<float> playerRect(m_player.getPosition(), sf::Vector2f{ 32.f, 48.f });

    bool show = false;

    if (m_hasNext && aabbOverlap(playerRect, m_zoneNext))
    {
        show = true;
        m_pressE.setString("Press E");
        // Affichage au-dessus/près du joueur (lisible)
        m_pressE.setPosition(m_zoneNext.position + sf::Vector2f{ 0.f, 220.f });

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::E))
        {
            if (onNextStation) onNextStation();
        }
    }
    else if (m_hasPrev && aabbOverlap(playerRect, m_zonePrev))
    {
        show = true;
        m_pressE.setString("Press E");
        m_pressE.setPosition(m_zonePrev.position + sf::Vector2f{ 0.f, 220.f });

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::E))
        {
            if (onPrevStation) onPrevStation();
        }
    }

    m_pressE.setFillColor(sf::Color(255, 255, 255, show ? 255 : 0));
}

// --------------------------------------------
// DRAW
// --------------------------------------------
void Level::draw(sf::RenderWindow& window)
{
    if (m_sceneTexPtr)
    {
        sf::Sprite bg(*m_sceneTexPtr);
        bg.setPosition(sf::Vector2f{ 0.f, 0.f });
        window.draw(bg);
    }

    if (m_hasPrev) window.draw(m_panelPrev);
    if (m_hasNext) window.draw(m_panelNext);

    if (m_pressE.getFillColor().a > 0)
        window.draw(m_pressE);

    m_player.draw(window);
}
