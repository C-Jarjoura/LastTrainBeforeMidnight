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

// widgets
#include "HintIcons.h"
#include "LevelInput.h"
#include "NpcInteraction.h"
#include "SceneNavigation.h"

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

    // zones (panneaux + train)
    sf::Rect<float> m_zoneNext;
    sf::Rect<float> m_zonePrev;
    sf::Rect<float> m_zoneTrain;

    // sol + limites horizontales
    float m_groundY = 540.f;
    float m_minX = 0.f;
    float m_maxX = 1920.f;

    // fader & pending switch
    ScreenFader m_fader;
    bool  m_sceneChangePending = false;
    int   m_nextSceneId = 1;

    // --- widgets ---
    HintIcons       m_hintIcons;
    LevelInput      m_input;
    NpcInteraction  m_npcInteraction;
    SceneNavigation m_navigation;

    // états dérivés pour la frame
    bool m_hasNext = false;
    bool m_hasPrev = false;
    bool m_hasTrain = false;

    // --- audio ambience smoothing (dialogue) ---
    // On ne touche pas NpcInteraction ; on lit juste l'état du DialogueSystem.
    bool  m_dialoguePrevActive = false;

    // Pitch (1.0 normal, 0.97 pendant dialogue)
    float m_targetPitch = 1.f;
    float m_currentPitch = 1.f;

    // Volume duck (0..100), lissé : 40% normal, 28% pendant dialogue
    float m_targetVolume = 40.f;
    float m_currentVolume = 40.f;
};
