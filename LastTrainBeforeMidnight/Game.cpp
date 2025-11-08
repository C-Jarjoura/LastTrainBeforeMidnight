#include "Game.h"
#include <SFML/Window/Keyboard.hpp>

// chemins de scènes
static const char* SCENE1 = "assets/scenes/scene_1.png";
static const char* SCENE2 = "assets/scenes/scene_2.png";
static const char* SCENE3 = "assets/scenes/scene_3.png";
static const char* SCENE_END = "assets/scenes/scene_end.png";

Game::Game()
    : m_window(sf::VideoMode({ 1920,1080 }), "Dernier Metro Avant Minuit")
{
    // callback demandé par Level quand le fade est noir :
    // -> Game charge la texture et appelle setScene()
    m_level.onRequestSceneTexture = [this](int id)
        {
            // charge dans m_sceneTex (ou end) puis renvoie au Level
            if (id == 99)
            {
                // écran de fin
                m_sceneEndTex = sf::Texture(SCENE_END);
                m_level.setScene(id, m_sceneEndTex);
                m_currentScene = 99;
            }
            else
            {
                // scène standard 1..3
                loadSceneTexture(id, m_sceneTex);
                m_level.setScene(id, m_sceneTex);
                m_currentScene = id;
            }
        };

    // fixe la station gagnante
    m_level.setWinningStation(3);

    // boot: charge la scène 1
    loadSceneTexture(1, m_sceneTex);
    m_currentScene = 1;
    m_level.setScene(1, m_sceneTex);
}

void Game::loadSceneTexture(int id, sf::Texture& out)
{
    if (id == 1)      out = sf::Texture(SCENE1);
    else if (id == 2) out = sf::Texture(SCENE2);
    else if (id == 3) out = sf::Texture(SCENE3);
    else              out = sf::Texture(SCENE_END); // fallback
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
    m_level.update(dt);
}

void Game::render()
{
    m_window.clear();
    m_level.draw(m_window);
    m_window.display();
}
