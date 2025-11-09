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

    enum class GameState
    {
        Menu,
        Playing
    };

private:
    sf::RenderWindow m_window;

    GameState m_state = GameState::Menu;

    int m_currentScene = 1;

    sf::Texture m_sceneTex;
    sf::Texture m_sceneEndTex;

    Level     m_level;
    MainMenu  m_menu;

    SoundBank m_sound;

    sf::Music m_music;
};
