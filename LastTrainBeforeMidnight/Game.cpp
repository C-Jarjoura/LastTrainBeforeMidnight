#include "Game.h"
#include "Level.h"

Game::Game()
    : m_window(sf::VideoMode({ 1920,1080 }), "Dernier Metro Avant Minuit"),
    m_isRunning(true)
{
}

void Game::run()
{
    sf::Clock clock;

    while (m_isRunning && m_window.isOpen())
    {
        processEvents();

        float dt = clock.restart().asSeconds();
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
    m_window.clear(sf::Color::Black);

    m_level.draw(m_window);

    m_window.display();
}
