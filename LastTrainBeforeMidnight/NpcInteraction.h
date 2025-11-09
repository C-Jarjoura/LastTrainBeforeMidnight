#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "DialogueSystem.h"
#include "NPC.h"
#include "Player.h"

class NpcInteraction
{
public:
    explicit NpcInteraction(DialogueSystem& dlg);

    // renvoie l'index du NPC "survolé" (colonne verticale), -1 sinon
    int computeHoverNpc(const sf::Rect<float>& playerRect,
        const std::vector<NPC>& npcs) const;

    // tente de démarrer un dialogue avec le npcIndex (si >=0)
    // repositionne le joueur pour libérer la vue du NPC
    // renvoie true si dialogue démarré
    bool tryBeginDialogue(int npcIndex,
        int sceneId,
        float groundY,
        Player& player,
        const std::vector<NPC>& npcs);

    // avance/ferme le dialogue si actif
    void handleAdvance(bool spaceEdge, bool eEdge);

    bool inDialogue() const { return m_inDialogue; }
    int  activeNpcIndex() const { return m_activeNpcIndex; }

private:
    DialogueSystem& m_dialogue;
    bool m_inDialogue = false;
    int  m_activeNpcIndex = -1;

private:
    static std::vector<std::string> loadSceneDialog(int sceneId);
};
