#include "Game.h"
#include <SFML/Window/Keyboard.hpp>

// paths
static const char* SCENE1 = "assets/scenes/scene_1.png";
static const char* SCENE2 = "assets/scenes/scene_2.png";
static const char* SCENE3 = "assets/scenes/scene_3.png";

Game::Game()
    : m_window(sf::VideoMode({ 1920,1080 }), "Dernier Metro Avant Minuit")
{
    // callback du LEVEL → GAME
    m_level.onRequestSceneTexture = [this](int newId)
        {
            // on charge ici
            loadSceneTexture(newId);

            // puis on applique texture + id au Level
            m_level.setScene(newId, m_sceneTex);
        };

    // première scène
    loadSceneTexture(1);
    m_level.setScene(1, m_sceneTex);
}

void Game::loadSceneTexture(int id)
{
    m_currentScene = id;

    if (id == 1)      m_sceneTex = sf::Texture(SCENE1);
    else if (id == 2) m_sceneTex = sf::Texture(SCENE2);
    else              m_sceneTex = sf::Texture(SCENE3);
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
    while (auto e = m_window.pollEvent())
    {
        if (e->is<sf::Event::Closed>())
            m_window.close();

        if (const auto* k = e->getIf<sf::Event::KeyPressed>())
        {
            if (k->scancode == sf::Keyboard::Scancode::Escape)
                m_window.close();
        }
    }
}

void Game::update(float dt)
{
    m_level.update(dt);
}

void Game::render()
{
    m_window.clear();
    m_level.draw(m_window);
    m_window.display();
}
