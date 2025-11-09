#include "NpcInteraction.h"
#include <fstream>
#include <string>

NpcInteraction::NpcInteraction(DialogueSystem& dlg)
    : m_dialogue(dlg)
{
}

int NpcInteraction::computeHoverNpc(const sf::Rect<float>& playerRect,
    const std::vector<NPC>& npcs) const
{
    for (int i = 0; i < static_cast<int>(npcs.size()); ++i)
    {
        auto np = npcs[i].getPosition();
        sf::Rect<float> npcZone({ np.x - 20.f, 0.f }, { 40.f, 2000.f });
        const bool overlap =
            (playerRect.position.x < npcZone.position.x + npcZone.size.x) &&
            (playerRect.position.x + playerRect.size.x > npcZone.position.x) &&
            (playerRect.position.y < npcZone.position.y + npcZone.size.y) &&
            (playerRect.position.y + playerRect.size.y > npcZone.position.y);

        if (overlap) return i;
    }
    return -1;
}

bool NpcInteraction::tryBeginDialogue(int npcIndex,
    int sceneId,
    float groundY,
    Player& player,
    const std::vector<NPC>& npcs)
{
    if (npcIndex < 0 || npcIndex >= static_cast<int>(npcs.size()))
        return false;

    // pousser le joueur pour ne pas cacher le NPC
    auto npcPos = npcs[npcIndex].getPosition();
    if (player.getPosition().x < npcPos.x)
        player.setPosition({ npcPos.x - 150.f, groundY });
    else
        player.setPosition({ npcPos.x + 150.f, groundY });

    // charger lignes
    auto lines = loadSceneDialog(sceneId);
    if (lines.empty())
    {
        m_dialogue.end();
        m_inDialogue = false;
        m_activeNpcIndex = -1;
        return false;
    }

    m_dialogue.startWithLines(lines);
    m_inDialogue = true;
    m_activeNpcIndex = npcIndex;
    return true;
}

void NpcInteraction::handleAdvance(bool spaceEdge, bool eEdge)
{
    if (!m_inDialogue) return;

    if (spaceEdge || eEdge)
    {
        m_dialogue.advance();
        if (!m_dialogue.active())
        {
            m_dialogue.end();
            m_inDialogue = false;
            m_activeNpcIndex = -1;
        }
    }
}

std::vector<std::string> NpcInteraction::loadSceneDialog(int sceneId)
{
    std::vector<std::string> r;
    std::ifstream f("assets/dialogue/scene" + std::to_string(sceneId) + ".dlg");
    if (!f.is_open()) return r;
    std::string line;
    while (std::getline(f, line)) if (!line.empty()) r.push_back(line);
    return r;
}
