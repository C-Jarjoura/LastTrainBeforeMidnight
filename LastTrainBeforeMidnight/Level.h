#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>
#include <string>
#include <cstdint>

#include "Player.h"
#include "NPC.h"
#include "DialogueSystem.h"

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

private:

    // === scene
    const sf::Texture* m_sceneTexPtr = nullptr;
    int   m_sceneId = 1;

    Player m_player;
    std::vector<NPC> m_npcs;

    // === panneaux
    sf::Texture m_panelTex;
    sf::Sprite  m_panelNext;
    sf::Sprite  m_panelPrev;

    // === icône E
    sf::Texture m_keyTex;
    sf::Sprite  m_keySpritePanel;
    sf::Sprite  m_keySpriteNPC;

    // === dialogue system (extracted)
    sf::Font m_font;
    DialogueSystem m_dialogue;

    bool m_hasNext = false;
    bool m_hasPrev = false;

    sf::Rect<float> m_zoneNext;
    sf::Rect<float> m_zonePrev;

    float m_groundY = 540.f;
    float m_minX = 0.f;
    float m_maxX = 1920.f;

    bool m_spaceWasDown = false;
    bool m_eWasDown = false;

    int m_panelHintAlpha = 0;
    int m_npcHintAlpha = 0;
};
