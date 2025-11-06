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

private:
    sf::RenderWindow m_window;
    bool m_isRunning;

    Level m_level;   // <--- AJOUT ICI
};
