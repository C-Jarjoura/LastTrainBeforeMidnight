#include "Game.h"

Game::Game()
    : m_window(sf::VideoMode({ 1920,1080 }), "Dernier Metro Avant Minuit"),
    m_isRunning(true)
{
}

void Game::run()
{
    while (m_isRunning && m_window.isOpen())
    {
        processEvents();
        update();
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


void Game::update()
{
    // update logic later
}

void Game::render()
{
    m_window.clear();
    // draw later
    m_window.display();
}
