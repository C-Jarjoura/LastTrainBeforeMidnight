#pragma once

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "NPC.h"

class Level
{
public:
    Level();

    void update(float dt);
    void draw(sf::RenderWindow& window);

private:
    Player m_player;
    std::vector<NPC*> m_npcs;
};
