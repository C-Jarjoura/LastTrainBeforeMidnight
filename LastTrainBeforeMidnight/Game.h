#pragma once
#include <SFML/Graphics.hpp>
#include "Level.h"

class Game
{
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();

    // ---- NEW ----
    void loadSceneTexture(int id);

private:
    sf::RenderWindow m_window;

    // ---- NEW ----
    int          m_currentScene = 1;
    sf::Texture  m_sceneTex;

    Level        m_level;
};
