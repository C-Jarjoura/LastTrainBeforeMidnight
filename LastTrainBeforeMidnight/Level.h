#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>
#include <string>
#include <cstdint>

#include "Player.h"
#include "NPC.h"
#include "DialogueSystem.h"
#include "ScreenFader.h"

class Level
{
public:
    Level();

    // Game donne la texture de fond (déjà chargée) pour id (1..3, 99=end)
    void setScene(int id, const sf::Texture& tex);

    // Level demande à Game la prochaine texture (quand fade est noir)
    std::function<void(int)> onRequestSceneTexture;

    void update(float dt);
    void draw(sf::RenderWindow& window);

    // fixe la station gagnante (par défaut 3)
    void setWinningStation(int id) { m_winningStation = id; }

private:
    // overlap Rect<float> (position + size in SFML3)
    bool aabbOverlap(const sf::Rect<float>& a, const sf::Rect<float>& b);

    // dialogue
    void beginDialogueForNpc(int npcIndex);
    void advanceDialogue();
    void closeDialogue();

    // demande un changement de scène avec fade
    void requestSceneChange(int newId);

private:
    // --- scène courante ---
    const sf::Texture* m_sceneTexPtr = nullptr;
    int   m_sceneId = 1;

    // gagnant
    int   m_winningStation = 3;

    Player m_player;
    std::vector<NPC> m_npcs;

    // panneaux
    sf::Texture m_panelTex;
    sf::Sprite  m_panelNext;
    sf::Sprite  m_panelPrev;

    // key icon
    sf::Texture m_keyTex;
    sf::Sprite  m_keySpritePanel;
    sf::Sprite  m_keySpriteNPC;
    sf::Sprite  m_keySpriteTrain;

    // font & dialogue system
    sf::Font m_font;
    DialogueSystem m_dialogue;

    // flags
    bool m_hasNext = false;
    bool m_hasPrev = false;
    bool m_hasTrain = false;

    // zones (panneaux + train)
    sf::Rect<float> m_zoneNext;
    sf::Rect<float> m_zonePrev;
    sf::Rect<float> m_zoneTrain;

    // sol + limites horizontales
    float m_groundY = 540.f;
    float m_minX = 0.f;
    float m_maxX = 1920.f;

    // état dialogue
    bool m_inDialogue = false;
    int  m_activeNpcIndex = -1;

    // input edges
    bool m_spaceWasDown = false;
    bool m_eWasDown = false;

    // hints alpha
    int m_panelHintAlpha = 0; // 0..255
    int m_npcHintAlpha = 0; // 0..255
    int m_trainHintAlpha = 0; // 0..255

    // fader & pending switch
    ScreenFader m_fader;
    bool  m_sceneChangePending = false;
    int   m_nextSceneId = 1;
};
