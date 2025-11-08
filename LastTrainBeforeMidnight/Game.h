#pragma once
#include <SFML/Graphics.hpp>
#include <string>
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

    // gestion des scènes
    void loadScene(int id);

private:
    sf::RenderWindow m_window;
    bool m_isRunning = true;

    // scène courante (1..3)
    int m_currentScene = 1;

    // la texture de la scène (Level en reçoit un pointeur)
    sf::Texture m_sceneTex;

    // le niveau (affiche la scène, le panneau, le joueur)
    Level m_level;
};
