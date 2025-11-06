#include "Level.h"

Level::Level()
    : m_map("assets/levels/tileset.png", 64)
{
    m_map.loadFromCSV("assets/levels/level1.csv");

    m_npcs.push_back(new NPC("assets/sprites/npc1.png", true, false, { 500.f, 400.f }));
    m_npcs.push_back(new NPC("assets/sprites/npc3.png", true, true, { 1200.f, 430.f }));
}


void Level::update(float dt)
{
    m_player.update(dt);

    for (auto* npc : m_npcs)
        npc->update(dt);
}

void Level::draw(sf::RenderWindow& window)
{
    window.draw(m_map);

    m_player.draw(window);

    for (auto* npc : m_npcs)
        npc->draw(window);
}
