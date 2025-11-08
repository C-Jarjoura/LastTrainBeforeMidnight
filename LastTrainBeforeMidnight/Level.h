#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>

#include "Player.h"
#include "NPC.h"

class Level
{
public:
    Level();

    void setScene(int id, const sf::Texture& tex);

    void update(float dt);
    void draw(sf::RenderWindow& window);

    std::function<void()> onNextStation;
    std::function<void()> onPrevStation;

private:
    bool aabbOverlap(const sf::Rect<float>& a, const sf::Rect<float>& b);
    // dans Level.h ajoute dans private:
    float m_minX;
    float m_maxX;

    const sf::Texture* m_sceneTexPtr;

    Player m_player;

    // --- NPC
    std::vector<NPC> m_npcs;

    // --- panels
    sf::Texture m_panelTex;
    sf::Sprite  m_panelNext;
    sf::Sprite  m_panelPrev;

    sf::Font m_font;
    sf::Text m_pressE;

    bool m_hasNext;
    bool m_hasPrev;

    sf::Rect<float> m_zoneNext;
    sf::Rect<float> m_zonePrev;

    float m_groundY;

};
