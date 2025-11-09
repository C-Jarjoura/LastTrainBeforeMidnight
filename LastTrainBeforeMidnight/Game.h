#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Level.h"
#include "MainMenu.h"
#include "SoundBank.h"

class Game
{
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();
    void loadSceneTexture(int id, sf::Texture& out);

    enum class GameState { Menu, Playing };

private:
    sf::RenderWindow m_window;

    GameState m_state = GameState::Menu;

    int m_currentScene = 1;

    sf::Texture m_sceneTex;
    sf::Texture m_sceneEndTex;

    Level     m_level;
    MainMenu  m_menu;

    // SFX courts
    SoundBank m_sound;

    // Musique longue (streaming)
    sf::Music m_music;

    // --- Audio fade (global musique) ---
    // menu = 60%, scene switch = 5%, après fade-in = 30%
    float m_musicVolumeCurrent = 60.f;
    float m_musicVolumeTarget = 60.f;
    float m_musicFadeSpeed = 80.f; // vitesse du lissage (approche douce)

    // Quand on quitte le menu, on descend à 5%, puis on remontera à 30% après fade-in de Level
    bool  m_raiseToGameplayAfterFadeIn = false;
};
