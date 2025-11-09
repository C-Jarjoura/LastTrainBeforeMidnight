#include "Game.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Audio.hpp>
// Exposés globaux pour autres modules
SoundBank* gSound = nullptr;
sf::Music* gMusic = nullptr;

// chemins de scènes
static const char* SCENE1 = "assets/scenes/scene_1.png";
static const char* SCENE2 = "assets/scenes/scene_2.png";
static const char* SCENE3 = "assets/scenes/scene_3.png";
static const char* SCENE_END = "assets/scenes/scene_end.png";

Game::Game()
    : m_window(sf::VideoMode({ 1920,1080 }), "Dernier Metro Avant Minuit")
{
    // SFX accessibles globalement si besoin
    gSound = &m_sound;

    // Charge tes SFX (optionnel si tu ne les utilises pas encore)
    m_sound.load("menu_move", "assets/sfx/menu_move.wav");
    m_sound.load("menu_enter", "assets/sfx/menu_enter.wav");

    // Musique : démarre dès le menu et ne s'arrête jamais (loop)
    // Tu as confirmé : file = assets/sfx/menu.wav
    if (!m_music.openFromFile("assets/sfx/menu.wav"))
    {
        // si le fichier manque, on ne crashe pas
        // (tu peux loguer si tu veux)
    }
    else
    {
        m_music.setLooping(true);   // SFML3 : setLooping
        m_music.setVolume(40.f);    // volume doux
        m_music.play();
    }
    gMusic = &m_music;

    // callback demandé par Level quand le fade est noir
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

    // station gagnante
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

    // JEU
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
