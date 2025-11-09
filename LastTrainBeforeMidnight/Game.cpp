#include "Game.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Audio.hpp>

// Exposés globaux pour autres modules
SoundBank* gSound = nullptr;
sf::Music* gMusic = nullptr;

// Signal volume demandé par Level (scene switch)
float g_pendingSceneVolume = -1.f;

// chemins de scènes
static const char* SCENE1 = "assets/scenes/scene_1.png";
static const char* SCENE2 = "assets/scenes/scene_2.png";
static const char* SCENE3 = "assets/scenes/scene_3.png";
static const char* SCENE_END = "assets/scenes/scene_end.png";

Game::Game()
    : m_window(sf::VideoMode({ 1920,1080 }), "Dernier Metro Avant Minuit")
{
    // SFX accessibles globalement
    gSound = &m_sound;

    // (optionnel) sfx de menu
    m_sound.load("menu_move", "assets/sfx/menu_move.wav");
    m_sound.load("menu_enter", "assets/sfx/menu_enter.wav");

    // Musique : démarre dès le menu et ne s'arrête jamais (loop)
    if (!m_music.openFromFile("assets/sfx/menu.wav"))
    {
        // fichier manquant : on ignore
    }
    else
    {
        m_music.setLooping(true);            // SFML3
        m_music.setVolume(m_musicVolumeCurrent); // 60% au menu
        m_music.play();
    }
    gMusic = &m_music;

    // Level demande la prochaine texture quand l'écran est noir
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
    // --- MENU ---
    if (m_state == GameState::Menu)
    {
        int r = m_menu.update(dt);

        if (r == 1) // jouer
        {
            // audio : on descend vers 5% pendant la transition vers le jeu
            m_musicVolumeTarget = 5.f;
            m_raiseToGameplayAfterFadeIn = true; // on remontera à 30% après fade-in visuel du Level

            // bascule état jeu
            m_state = GameState::Playing;

            // boot scène 1
            loadSceneTexture(1, m_sceneTex);
            m_currentScene = 1;
            m_level.setScene(1, m_sceneTex);
        }
        else if (r == 2) // quitter
        {
            m_window.close();
        }

        // smoothing volume en menu
        if (gMusic)
        {
            // approche douce (exponentielle) vers target
            m_musicVolumeCurrent += (m_musicVolumeTarget - m_musicVolumeCurrent) * m_musicFadeSpeed * dt / 100.f;
            gMusic->setVolume(m_musicVolumeCurrent);
        }

        m_sound.garbageCollect();
        return;
    }

    // --- PLAYING ---
    m_level.update(dt);

    // Level peut signaler un changement de volume (scene switch -> 5%) via g_pendingSceneVolume
    if (g_pendingSceneVolume >= 0.f)
    {
        m_musicVolumeTarget = g_pendingSceneVolume;
        g_pendingSceneVolume = -1.f;
    }

    // si on vient de quitter le menu, on remontera à 30% uniquement quand le fade-in de Level est terminé
    // On détecte la fin de fade-in en lisant le signal posé par Level (voir Level.cpp plus bas).
    // Ici, Game ne le sait pas directement, donc Level fera : g_pendingSceneVolume = 30.f à la fin du fade-in.
    // (pas besoin de méthode publique)
    // => donc rien à faire ici, juste écouter g_pendingSceneVolume.

    // smoothing volume en jeu
    if (gMusic)
    {
        m_musicVolumeCurrent += (m_musicVolumeTarget - m_musicVolumeCurrent) * m_musicFadeSpeed * dt / 100.f;
        gMusic->setVolume(m_musicVolumeCurrent);
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
