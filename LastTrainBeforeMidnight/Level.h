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

    sf::Texture m_scene1;
    sf::Texture m_panelTex;

    sf::Sprite m_sceneSprite;
    sf::Sprite m_panelSprite;

    sf::Rect<float> m_panelZone;
};
