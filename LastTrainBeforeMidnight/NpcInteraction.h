#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <unordered_set>
#include <array>

#include "DialogueSystem.h"
#include "NPC.h"
#include "Player.h"

// Gestion de l'interaction NPC : gating par flags, déroulé par étapes, attribution de flags à la fin du dialogue.
class NpcInteraction
{
public:
    // flags = référence détenue par Level
    explicit NpcInteraction(DialogueSystem& dlg, std::unordered_set<std::string>& flags);

    // renvoie l'index du NPC "survolé" (colonne verticale), -1 sinon
    int computeHoverNpc(const sf::Rect<float>& playerRect,
        const std::vector<NPC>& npcs) const;

    // tente de démarrer un dialogue (repositionne juste en X)
    // renvoie true si dialogue démarré
    bool tryBeginDialogue(int npcIndex,
        int sceneId,
        float /*groundY*/,
        Player& player,
        const std::vector<NPC>& npcs);

    // avance/ferme le dialogue si actif (si le dialogue se termine -> attribue le flag d'étape)
    void handleAdvance(bool spaceEdge, bool eEdge);

    // reset des étapes (nouveau run)
    void resetStages();

    bool inDialogue() const { return m_inDialogue; }
    int  activeNpcIndex() const { return m_activeNpcIndex; }

private:
    // parse un .dlg avec SAY/COND/END_COND/SETFLAG, en tenant compte des flags
    std::vector<std::string> loadSceneDialog(int sceneId);

    // gating d'accès par scène (pré-requis globaux pour que le NPC accepte de parler)
    bool sceneGateSatisfied(int sceneId) const;

    // prérequis d'ÉTAPE (Option B : inter-dépendances entre scènes)
    bool stagePrereqSatisfied(int sceneId, int stage) const;

    // attribution du flag correspondant à (sceneId, stage)
    void awardStageFlag(int sceneId, int stage);

private:
    DialogueSystem& m_dialogue;
    std::unordered_set<std::string>& m_flags;

    bool m_inDialogue = false;
    int  m_activeNpcIndex = -1;

    // étape courante par scène (1..3, 99=end non utilisé)
    // sceneId ∈ {1,2,3} -> index = sceneId (on ignore 0 et 99)
    std::array<int, 100> m_sceneStage{}; // init à 0
    int  m_pendingSceneForAward = -1;    // scène du dialogue en cours pour award à la fin
};
