#pragma once

#include <SFML/Graphics.hpp>
#include "Player.h"

class Level
{
public:
    Level();

    void update(float dt);
    void draw(sf::RenderWindow& window);

private:
    Player m_player;
};