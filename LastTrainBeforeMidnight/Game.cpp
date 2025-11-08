#include "Game.h"
#include <SFML/Window/Keyboard.hpp>

// chemins des scènes (1920x1080)
static const char* SCENE1 = "assets/scenes/scene_1.png";
static const char* SCENE2 = "assets/scenes/scene_2.png";
static const char* SCENE3 = "assets/scenes/scene_3.png";

Game::Game()
    : m_window(sf::VideoMode({ 1920,1080 }), "Dernier Metro Avant Minuit")
    , m_isRunning(true)
{
    // brancher les callbacks du Level -> Game
    m_level.onNextStation = [this]()
        {
            if (m_currentScene < 3) loadScene(m_currentScene + 1);
        };
    m_level.onPrevStation = [this]()
        {
            if (m_currentScene > 1) loadScene(m_currentScene - 1);
        };

    // charger la première scène
    loadScene(1);
}

void Game::loadScene(int id)
{
    m_currentScene = id;

    // charger la bonne texture de fond
    if (id == 1)      m_sceneTex = sf::Texture(SCENE1);
    else if (id == 2) m_sceneTex = sf::Texture(SCENE2);
    else              m_sceneTex = sf::Texture(SCENE3);

    // passer la scène au Level (par pointeur sur texture + id)
    m_level.setScene(id, m_sceneTex);
}

void Game::run()
{
    sf::Clock clock;
    while (m_isRunning && m_window.isOpen())
    {
        processEvents();

        const float dt = clock.restart().asSeconds();
        update(dt);

        render();
    }
}

void Game::processEvents()
{
    while (auto event = m_window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            m_window.close();

        if (const auto* key = event->getIf<sf::Event::KeyPressed>())
        {
            if (key->scancode == sf::Keyboard::Scancode::Escape)
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
