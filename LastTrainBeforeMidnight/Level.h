#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>

#include "Player.h"
#include "NPC.h"
#include "DialogueSystem.h"
#include "ScreenFader.h"

class Level
{
public:
    Level();

    void setScene(int id, const sf::Texture& tex);

    void update(float dt);
    void draw(sf::RenderWindow& window);

    void requestSceneChange(int newId);

    // Game appelle ceci quand la nouvelle texture est prête
    std::function<void(int)> onRequestSceneTexture;

private:
    bool aabbOverlap(const sf::Rect<float>& a, const sf::Rect<float>& b);

private:
    int  m_sceneId = 1;
    const sf::Texture* m_sceneTexPtr = nullptr;

    Player m_player;
    std::vector<NPC> m_npcs;

    // panels
    sf::Texture m_panelTex;
    sf::Sprite  m_panelNext;
    sf::Sprite  m_panelPrev;

    bool m_hasNext = false;
    bool m_hasPrev = false;
    sf::Rect<float> m_zoneNext;
    sf::Rect<float> m_zonePrev;

    // icons
    sf::Texture m_keyTex;
    sf::Sprite  m_keySpritePanel;
    sf::Sprite  m_keySpriteNPC;

    int m_panelHintAlpha = 0;
    int m_npcHintAlpha = 0;

    // dialogue
    sf::Font m_font;             // <<< correction
    DialogueSystem m_dialogue;   // <<< correction

    float m_groundY = 540.f;
    float m_minX = 0.f;
    float m_maxX = 1920.f;

    bool m_spaceWasDown = false;
    bool m_eWasDown = false;

    // fade
    ScreenFader m_fader;
    bool m_sceneChangePending = false;
    int  m_nextSceneId = 1;
};
