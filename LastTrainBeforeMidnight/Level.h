#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>
#include <string>
#include <cstdint>
#include <unordered_set>

#include "Player.h"
#include "NPC.h"
#include "DialogueSystem.h"
#include "ScreenFader.h"

// widgets
#include "HintIcons.h"
#include "LevelInput.h"
#include "NpcInteraction.h"
#include "SceneNavigation.h"

class Level
{
public:
    Level();

    void setScene(int id, const sf::Texture& tex);
    std::function<void(int)> onRequestSceneTexture;

    void update(float dt);
    void draw(sf::RenderWindow& window);

    void setWinningStation(int id) { m_winningStation = id; }
    void startNewRun();

private:
    void requestSceneChange(int newId);
    bool allFlagsCollected() const;

private:
    const sf::Texture* m_sceneTexPtr = nullptr;
    int   m_sceneId = 1;
    int   m_winningStation = 3;

    Player m_player;
    std::vector<NPC> m_npcs;

    sf::Texture m_panelTex;
    sf::Sprite  m_panelNext;
    sf::Sprite  m_panelPrev;

    sf::Texture m_keyTex;
    sf::Sprite  m_keySpritePanel;
    sf::Sprite  m_keySpriteNPC;
    sf::Sprite  m_keySpriteTrain;

    sf::Font m_font;
    DialogueSystem m_dialogue;

    sf::Rect<float> m_zoneNext;
    sf::Rect<float> m_zonePrev;
    sf::Rect<float> m_zoneTrain;

    float m_groundY = 540.f;
    float m_minX = 0.f;
    float m_maxX = 1920.f;

    ScreenFader m_fader;
    bool  m_sceneChangePending = false;
    int   m_nextSceneId = 1;

    HintIcons       m_hintIcons;
    LevelInput      m_input;
    NpcInteraction  m_npcInteraction;
    SceneNavigation m_navigation;

    bool m_hasNext = false;
    bool m_hasPrev = false;
    bool m_hasTrain = false;

    bool  m_dialoguePrevActive = false;
    float m_targetPitch = 1.f;
    float m_currentPitch = 1.f;
    float m_targetVolume = 40.f;
    float m_currentVolume = 40.f;

    // progression
    std::unordered_set<std::string> m_flags;

    // timer 5:00
    float m_timer = 300.f;
};
