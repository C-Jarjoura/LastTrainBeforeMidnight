#include "Game.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Audio.hpp>
#include <cmath>

// exposés globaux
SoundBank* gSound = nullptr;
sf::Music* gMusic = nullptr;
float g_pendingSceneVolume = -1.f; // < 0 = pas de cible en attente

// chemins de scènes
static const char* SCENE1 = "assets/scenes/scene_1.png";
static const char* SCENE2 = "assets/scenes/scene_2.png";
static const char* SCENE3 = "assets/scenes/scene_3.png";
static const char* SCENE_END = "assets/scenes/scene_end.png";   // GOOD END (99)
static const char* SCENE_GAMEOVER = "assets/scenes/game_over.png";   // BAD  END (98)

Game::Game()
    : m_window(sf::VideoMode({ 1920,1080 }), "Dernier Metro Avant Minuit")
{
    gSound = &m_sound;

    // SFX UI menu
    m_sound.load("menu_move", "assets/sfx/menu_move.wav");
    m_sound.load("menu_enter", "assets/sfx/menu_enter.wav");

    // SFX dialogues (bleeps)
    m_sound.load("blipA", "assets/sfx/dialog_bleep1.wav");
    m_sound.load("blipB", "assets/sfx/dialog_bleep2.wav");
    m_sound.load("blipC", "assets/sfx/dialog_bleep3.wav");

    // Musique longue (menu + jeu, en fond)
    if (m_music.openFromFile("assets/sfx/menu.wav"))
    {
        m_music.setLooping(true);
        m_music.setVolume(60.f); // menu plus fort
        m_music.play();
        gMusic = &m_music;
    }

    // callback Level → Game texture swap
    m_level.onRequestSceneTexture = [this](int id)
        {
            if (id == 99) // GOOD END
            {
                m_sceneEndTex = sf::Texture(SCENE_END);
                m_level.setScene(id, m_sceneEndTex);
                m_currentScene = 99;
            }
            else if (id == 98) // GAME OVER
            {
                m_sceneEndTex = sf::Texture(SCENE_GAMEOVER);
                m_level.setScene(id, m_sceneEndTex);
                m_currentScene = 98;
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
    // lissage volume musique global via g_pendingSceneVolume
    if (gMusic && g_pendingSceneVolume >= 0.f)
    {
        float cur = gMusic->getVolume();
        float target = g_pendingSceneVolume;

        const float speed = 120.f; // % par seconde
        float step = speed * dt;

        if (cur < target) cur = (cur + step < target) ? (cur + step) : target;
        else              cur = (cur - step > target) ? (cur - step) : target;

        gMusic->setVolume(cur);

        if (std::abs(cur - target) < 0.5f)
        {
            gMusic->setVolume(target);
            g_pendingSceneVolume = -1.f; // cible atteinte
        }
    }

    // statiques pour endings
    static float overTimer = 4.f;   // game over auto-exit
    static bool  winStopped = false;

    // reset quand on n'est pas sur l'écran correspondant
    if (m_currentScene != 98) overTimer = 4.f;
    if (m_currentScene != 99) winStopped = false;

    if (m_state == GameState::Menu)
    {
        int r = m_menu.update(dt);

        if (r == 1) // jouer
        {
            m_state = GameState::Playing;

            // reset progression + timer du run
            m_level.startNewRun();

            // charger scène 1
            loadSceneTexture(1, m_sceneTex);
            m_currentScene = 1;
            m_level.setScene(1, m_sceneTex);

            // musique plus basse pour gameplay
            g_pendingSceneVolume = 30.f;
        }
        else if (r == 2) // quitter
        {
            m_window.close();
        }

        m_sound.garbageCollect();
        return;
    }

    // --- jeu ---
    m_level.update(dt);

    // --- GAME OVER (98) : fade audio vers 0 puis fermeture fenêtre après 4 sec ---
    if (m_currentScene == 98)
    {
        overTimer -= dt;

        if (gMusic)
        {
            float v = gMusic->getVolume();
            v -= dt * 20.f; // fade ~3s
            if (v < 0.f) v = 0.f;
            gMusic->setVolume(v);
        }

        if (overTimer <= 0.f)
        {
            m_window.close();
            return;
        }
    }

    // --- GOOD END (99) : fade audio vers 0 et stop (reste sur l'écran) ---
    if (m_currentScene == 99)
    {
        if (gMusic && !winStopped)
        {
            float v = gMusic->getVolume();
            v -= dt * 20.f;
            if (v <= 0.f)
            {
                v = 0.f;
                gMusic->setVolume(v);
                gMusic->stop();   // choix A : stopper en endings
                winStopped = true;
            }
            else
            {
                gMusic->setVolume(v);
            }
        }
    }

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
