#include "Level.h"

Level::Level()
{
    // plus tard on pourra push des NPC ici
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
