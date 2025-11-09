#include "NpcInteraction.h"
#include <fstream>
#include <string>
#include <stack>
#include <cstring>

NpcInteraction::NpcInteraction(DialogueSystem& dlg, std::unordered_set<std::string>& flags)
    : m_dialogue(dlg)
    , m_flags(flags)
{
    m_sceneStage.fill(0);
}

void NpcInteraction::resetStages()
{
    m_sceneStage.fill(0);
    m_inDialogue = false;
    m_activeNpcIndex = -1;
    m_pendingSceneForAward = -1;
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

// Gating global : qui a le droit de démarrer un dialogue ?
bool NpcInteraction::sceneGateSatisfied(int sceneId) const
{
    // règle : NPC scène2 requiert HEARD_NOISE ; NPC scène3 requiert LOST_WATCH ; scène1 = libre
    if (sceneId == 2) return m_flags.count("HEARD_NOISE") != 0;
    if (sceneId == 3) return m_flags.count("LOST_WATCH") != 0;
    return true;
}

// Prérequis d’étape (Option B : inter-dépendances)
bool NpcInteraction::stagePrereqSatisfied(int sceneId, int stage) const
{
    // stage = 'ce qu'on VA donner' (0->1ère fois, 1->2ème fois, ...)

    // mapping des étapes & dépendances (cf. design validé) :
    // Scene 1:
    //   stage 0 -> HEARD_NOISE (aucun prérequis)
    //   stage 1 -> GUARD_CLOSED (requiert LOST_WATCH)
    // Scene 2:
    //   stage 0 -> LOST_WATCH (aucun)
    //   stage 1 -> TRAIN4_FAKE (requiert LINE13_WARNING)
    // Scene 3:
    //   stage 0 -> LINE13_WARNING (aucun)
    //   stage 1 -> SEEN_SHADOW (requiert GUARD_CLOSED)
    //   stage 2 -> FINAL_CLUE  (requiert TRAIN4_FAKE)

    if (sceneId == 1)
    {
        if (stage == 0) return true;
        if (stage == 1) return m_flags.count("LOST_WATCH") != 0;
        return false; // pas d'autre étape
    }
    else if (sceneId == 2)
    {
        if (stage == 0) return true;
        if (stage == 1) return m_flags.count("LINE13_WARNING") != 0;
        return false;
    }
    else if (sceneId == 3)
    {
        if (stage == 0) return true;
        if (stage == 1) return m_flags.count("GUARD_CLOSED") != 0;
        if (stage == 2) return m_flags.count("TRAIN4_FAKE") != 0;
        return false;
    }
    return false;
}

void NpcInteraction::awardStageFlag(int sceneId, int stage)
{
    if (sceneId == 1)
    {
        if (stage == 0) m_flags.insert("HEARD_NOISE");
        else if (stage == 1) m_flags.insert("GUARD_CLOSED");
    }
    else if (sceneId == 2)
    {
        if (stage == 0) m_flags.insert("LOST_WATCH");
        else if (stage == 1) m_flags.insert("TRAIN4_FAKE");
    }
    else if (sceneId == 3)
    {
        if (stage == 0) m_flags.insert("LINE13_WARNING");
        else if (stage == 1) m_flags.insert("SEEN_SHADOW");
        else if (stage == 2) m_flags.insert("FINAL_CLUE");
    }
}

bool NpcInteraction::tryBeginDialogue(int npcIndex,
    int sceneId,
    float /*groundY*/,
    Player& player,
    const std::vector<NPC>& npcs)
{
    if (npcIndex < 0 || npcIndex >= static_cast<int>(npcs.size()))
        return false;

    // Gating global d'accès
    if (!sceneGateSatisfied(sceneId))
        return false;

    // Étape visée
    int stage = m_sceneStage[sceneId];

    // Prérequis de l'étape
    if (!stagePrereqSatisfied(sceneId, stage))
        return false;

    // libérer la vue : décalage X uniquement, garder Y
    auto npcPos = npcs[npcIndex].getPosition();
    float px = player.getPosition().x;
    float py = player.getPosition().y;

    if (px < npcPos.x) player.setPosition({ npcPos.x - 150.f, py });
    else               player.setPosition({ npcPos.x + 150.f, py });

    // Charger lignes (SAY/COND/END_COND/SETFLAG)
    auto lines = loadSceneDialog(sceneId);
    if (lines.empty())
        return false;

    m_dialogue.startWithLines(lines);
    m_inDialogue = true;
    m_activeNpcIndex = npcIndex;

    // Memoriser la scène pour award à la fermeture
    m_pendingSceneForAward = sceneId;
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
            // Dialogue FINI -> Award flag de l'étape
            if (m_pendingSceneForAward >= 1 && m_pendingSceneForAward < (int)m_sceneStage.size())
            {
                int s = m_sceneStage[m_pendingSceneForAward];
                awardStageFlag(m_pendingSceneForAward, s);
                // avancer l'étape pour la prochaine fois
                m_sceneStage[m_pendingSceneForAward] = s + 1;
            }

            // fermer état
            m_dialogue.end();
            m_inDialogue = false;
            m_activeNpcIndex = -1;
            m_pendingSceneForAward = -1;
        }
    }
}

std::vector<std::string> NpcInteraction::loadSceneDialog(int sceneId)
{
    std::vector<std::string> out;

    // On charge en fonction de la scène ET de l'étape courante, pour te permettre
    // de mettre des fichiers différents si tu veux : sceneX_stageY.dlg
    // Sinon, on retombe sur sceneX.dlg unique, qui peut contenir des COND/SETFLAG.
    const int stage = m_sceneStage[sceneId];

    // tente d'abord assets/dialogue/scene{sceneId}_stage{stage}.dlg
    {
        std::string alt = "assets/dialogue/scene" + std::to_string(sceneId) + "_stage" + std::to_string(stage) + ".dlg";
        std::ifstream f(alt);
        if (f.is_open())
        {
            std::string line;
            std::stack<bool> condStack;
            condStack.push(true);
            auto isActive = [&]() { return !condStack.empty() && condStack.top(); };

            while (std::getline(f, line))
            {
                // trim
                while (!line.empty() && (line.back() == '\r' || line.back() == '\n' || line.back() == ' ' || line.back() == '\t'))
                    line.pop_back();
                size_t p = 0; while (p < line.size() && (line[p] == ' ' || line[p] == '\t')) ++p;
                if (p > 0) line = line.substr(p);
                if (line.empty()) continue;

                if (line.rfind("COND:", 0) == 0)
                {
                    std::string key = line.substr(5);
                    bool ok = (m_flags.count(key) != 0);
                    condStack.push(isActive() && ok);
                    continue;
                }
                if (line == "END_COND")
                {
                    if (condStack.size() > 1) condStack.pop();
                    continue;
                }
                if (line.rfind("SETFLAG:", 0) == 0)
                {
                    if (isActive())
                    {
                        std::string key = line.substr(8);
                        if (!key.empty()) m_flags.insert(key);
                    }
                    continue;
                }
                if (line.rfind("SAY:", 0) == 0)
                {
                    if (isActive()) out.push_back(line.substr(4));
                    continue;
                }
                if (isActive()) out.push_back(line);
            }
            return out;
        }
    }

    // fallback: scene{sceneId}.dlg
    {
        std::string path = "assets/dialogue/scene" + std::to_string(sceneId) + ".dlg";
        std::ifstream f(path);
        if (!f.is_open()) return out;

        std::string line;
        std::stack<bool> condStack;
        condStack.push(true);
        auto isActive = [&]() { return !condStack.empty() && condStack.top(); };

        while (std::getline(f, line))
        {
            // trim
            while (!line.empty() && (line.back() == '\r' || line.back() == '\n' || line.back() == ' ' || line.back() == '\t'))
                line.pop_back();
            size_t p = 0; while (p < line.size() && (line[p] == ' ' || line[p] == '\t')) ++p;
            if (p > 0) line = line.substr(p);
            if (line.empty()) continue;

            if (line.rfind("COND:", 0) == 0)
            {
                std::string key = line.substr(5);
                bool ok = (m_flags.count(key) != 0);
                condStack.push(isActive() && ok);
                continue;
            }
            if (line == "END_COND")
            {
                if (condStack.size() > 1) condStack.pop();
                continue;
            }
            if (line.rfind("SETFLAG:", 0) == 0)
            {
                if (isActive())
                {
                    std::string key = line.substr(8);
                    if (!key.empty()) m_flags.insert(key);
                }
                continue;
            }
            if (line.rfind("SAY:", 0) == 0)
            {
                if (isActive()) out.push_back(line.substr(4));
                continue;
            }
            if (isActive()) out.push_back(line);
        }
    }

    return out;
}
