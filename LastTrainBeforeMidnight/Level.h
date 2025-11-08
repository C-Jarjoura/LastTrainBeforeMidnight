#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>
#include <string>

#include "Player.h"
#include "NPC.h"

class Level
{
public:
    Level();

    // Charge la scène (1..3) et la texture de fond associée
    void setScene(int id, const sf::Texture& tex);

    void update(float dt);
    void draw(sf::RenderWindow& window);

    // callbacks pour changer de station
    std::function<void()> onNextStation;
    std::function<void()> onPrevStation;

private:
    // utils
    bool aabbOverlap(const sf::Rect<float>& a, const sf::Rect<float>& b);

    // dialogues
    void beginDialogueForNpc(int npcIndex);
    void advanceDialogue(); // SPACE
    void closeDialogue();

private:
    // --- scène courante ---
    const sf::Texture* m_sceneTexPtr = nullptr;
    int   m_sceneId = 1;

    Player m_player;
    std::vector<NPC> m_npcs;

    // --- panneaux ---
    sf::Texture m_panelTex;
    sf::Sprite  m_panelNext;
    sf::Sprite  m_panelPrev;

    // UI hints
    sf::Font m_font;
    sf::Text m_pressE;       // panneau
    sf::Text m_pressE_NPC;   // npc

    bool m_hasNext = false;
    bool m_hasPrev = false;

    sf::Rect<float> m_zoneNext;
    sf::Rect<float> m_zonePrev;

    // sol + bordures horizontales
    float m_groundY = 540.f;
    float m_minX = 0.f;
    float m_maxX = 1920.f;

    // --- état dialogue ---
    bool m_inDialogue = false;
    int  m_activeNpcIndex = -1;
    std::vector<std::string> m_dialogLines;
    std::size_t m_dialogIndex = 0;

    // détection d’appui “edge”
    bool m_spaceWasDown = false;

    // sous-titre (texte centré en bas)
    sf::Text m_dialogText;
};
