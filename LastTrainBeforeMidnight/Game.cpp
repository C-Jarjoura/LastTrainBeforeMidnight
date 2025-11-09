#include "Game.h"
#include <SFML/Window/Keyboard.hpp>

// exposés globaux
SoundBank* gSound = nullptr;
sf::Music* gMusic = nullptr;
float g_pendingSceneVolume = -1.f;

// chemins de scènes
static const char* SCENE1 = "assets/scenes/scene_1.png";
static const char* SCENE2 = "assets/scenes/scene_2.png";
static const char* SCENE3 = "assets/scenes/scene_3.png";
static const char* SCENE_END = "assets/scenes/scene_end.png";

Game::Game()
    : m_window(sf::VideoMode({ 1920,1080 }), "Dernier Metro Avant Minuit")
{
    gSound = &m_sound;

    // menu UI sfx
    m_sound.load("menu_move", "assets/sfx/menu_move.wav");
    m_sound.load("menu_enter", "assets/sfx/menu_enter.wav");

    // --- dialogue bleeps ---
    m_sound.load("blipA", "assets/sfx/dialog_bleep1.wav");
    m_sound.load("blipB", "assets/sfx/dialog_bleep2.wav");
    m_sound.load("blipC", "assets/sfx/dialog_bleep3.wav");

    // musique menu & jeu
    if (m_music.openFromFile("assets/sfx/menu.wav"))
    {
        m_music.setLooping(true);
        m_music.setVolume(40.f);
        m_music.play();
        gMusic = &m_music;
    }

    // callback Level → Game texture swap
    m_level.onRequestSceneTexture = [this](int id)
        {
            if (id == 99)
            {
                m_sceneEndTex = sf::Texture(SCENE_END);
                m_level.setScene(id, m_sceneEndTex);
                m_currentScene = 99;
            }
            else
            {
                loadSceneTexture(id, m_sceneTex);
                m_level.setScene(id, m_sceneTex);
                m_currentScene = id;
            }
        };

    m_level.setWinningStation(3);
}

void Game::loadSceneTexture(int id, sf::Texture& out)
{
    if (id == 1)      out = sf::Texture(SCENE1);
    else if (id == 2) out = sf::Texture(SCENE2);
    else if (id == 3) out = sf::Texture(SCENE3);
    else              out = sf::Texture(SCENE_END);
}

void Game::run()
{
    sf::Clock clock;
    while (m_window.isOpen())
    {
        processEvents();
        float dt = clock.restart().asSeconds();
        update(dt);
        render();
    }
}

void Game::processEvents()
{
    while (auto ev = m_window.pollEvent())
    {
        if (ev->is<sf::Event::Closed>())
            m_window.close();

        if (const auto* key = ev->getIf<sf::Event::KeyPressed>())
        {
            if (key->scancode == sf::Keyboard::Scancode::Escape)
                m_window.close();
        }
    }
}

void Game::update(float dt)
{
    if (m_state == GameState::Menu)
    {
        int r = m_menu.update(dt);

        if (r == 1) // jouer
        {
            m_state = GameState::Playing;
            loadSceneTexture(1, m_sceneTex);
            m_currentScene = 1;
            m_level.setScene(1, m_sceneTex);
        }
        else if (r == 2) // quitter
        {
            m_window.close();
        }

        m_sound.garbageCollect();
        return;
    }

    // JEUX
    m_level.update(dt);
    m_sound.garbageCollect();
}

void Game::render()
{
    m_window.clear();

    if (m_state == GameState::Menu)
        m_menu.draw(m_window);
    else
        m_level.draw(m_window);

    m_window.display();
}
