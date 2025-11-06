#include "Level.h"

Level::Level()
{
    // Ajout des 3 NPC
    m_npcs.push_back(new NPC("npc1.png", true, false, sf::Vector2f{ 500.f, 400.f }));
    m_npcs.push_back(new NPC("npc2.png", false, false, sf::Vector2f{ 850.f, 350.f }));
    m_npcs.push_back(new NPC("npc3.png", true, true, sf::Vector2f{ 1200.f, 430.f }));
}

void Level::update(float dt)
{
    m_player.update(dt);

    for (auto* npc : m_npcs)
        npc->update(dt);
}

void Level::draw(sf::RenderWindow& window)
{
    m_player.draw(window);

    for (auto* npc : m_npcs)
        npc->draw(window);
}
