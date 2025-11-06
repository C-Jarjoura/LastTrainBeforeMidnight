#pragma once

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "TileMap.h"
#include "NPC.h"        // <-- AJOUT OBLIGATOIRE

class Level
{
public:
    Level();

    void update(float dt);
    void draw(sf::RenderWindow& window);

private:
    std::vector<NPC*> m_npcs;
    TileMap m_map;
    Player m_player;
};
