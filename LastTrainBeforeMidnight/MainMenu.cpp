#include "MainMenu.h"
#include <SFML/Window/Keyboard.hpp>
#include "SoundBank.h"

// petit pointeur global pour éviter l’injection partout (déjà utilisé côté Game)
extern SoundBank* gSound;

static float pulseTimer = 0.f;

MainMenu::MainMenu()
    : m_bgTex("assets/scenes/menu.png")
    , m_bgSprite(m_bgTex)
{
    // surbrillance semi-transparente (pixels JOUER/QUITTER)
    m_highlight.setFillColor(sf::Color(255, 255, 255, 60));
    m_highlight.setSize({ 1255.f,262.f }); // valeurs relevées

    updateHighlight();
}

void MainMenu::updateHighlight()
{
    float baseX = 333.f;
    float y = (m_selected == 0) ? 182.f : 504.f; // jouer / quitter
    m_highlight.setPosition({ baseX, y });
}

int MainMenu::update(float dt)
{
    (void)dt;

    bool up = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up);
    bool down = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down);

    static bool prevUp = false;
    static bool prevDown = false;

    if (up && !prevUp)
    {
        m_selected = 0;
        updateHighlight();
        if (gSound) gSound->playOneShot("menu_move", 80.f);
    }
    if (down && !prevDown)
    {
        m_selected = 1;
        updateHighlight();
        if (gSound) gSound->playOneShot("menu_move", 80.f);
    }

    prevUp = up;
    prevDown = down;

    // validation
    bool validate =
        sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Enter) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::E);

    if (validate)
    {
        if (gSound) gSound->playOneShot("menu_enter", 85.f);
        return (m_selected == 0) ? 1 : 2;
    }

    // pulse breathing
    pulseTimer += dt;
    float s = 1.f + std::sin(pulseTimer * 3.f) * 0.03f;
    m_highlight.setScale({ s, s });

    return -1;
}

void MainMenu::draw(sf::RenderWindow& window)
{
    window.draw(m_bgSprite);
    window.draw(m_highlight);
}
