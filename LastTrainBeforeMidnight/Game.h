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

    // charge la texture correspondant à l'id (1..3, 99=end)
    void loadSceneTexture(int id, sf::Texture& out);

private:
    sf::RenderWindow m_window;

    // scène courante (1..3, 99 = end)
    int m_currentScene = 1;

    // texture de fond active
    sf::Texture m_sceneTex;

    // texture fin
    sf::Texture m_sceneEndTex;

    Level m_level;
};
