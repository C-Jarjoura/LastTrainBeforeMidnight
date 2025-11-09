#include "HintIcons.h"
#include <algorithm>
#include <cstdint>

static inline uint8_t clampToByte(int v)
{
    if (v < 0) return 0;
    if (v > 255) return 255;
    return static_cast<uint8_t>(v);
}

HintIcons::HintIcons(sf::Sprite& panelIcon, sf::Sprite& npcIcon, sf::Sprite& trainIcon)
    : m_panel(panelIcon), m_npc(npcIcon), m_train(trainIcon)
{
    // démarrer invisibles
    auto p = m_panel.getColor(); p.a = 0; m_panel.setColor(p);
    auto n = m_npc.getColor();   n.a = 0; m_npc.setColor(n);
    auto t = m_train.getColor(); t.a = 0; m_train.setColor(t);
}

int HintIcons::approachAlpha(int current, int target, float k)
{
    // interpolation exponentielle simple
    const float v = current + (target - current) * k;
    return static_cast<int>(v);
}

void HintIcons::update(float dt, bool showPanel, bool showNpc, bool showTrain)
{
    const float k = m_fadeSpeed * dt;

    m_panelAlpha = approachAlpha(m_panelAlpha, showPanel ? 255 : 0, k);
    m_npcAlpha = approachAlpha(m_npcAlpha, showNpc ? 255 : 0, k);
    m_trainAlpha = approachAlpha(m_trainAlpha, showTrain ? 255 : 0, k);

    m_panelAlpha = std::clamp(m_panelAlpha, 0, 255);
    m_npcAlpha = std::clamp(m_npcAlpha, 0, 255);
    m_trainAlpha = std::clamp(m_trainAlpha, 0, 255);

    // appliquer couleurs si visibles
    if (m_panelAlpha > 0)
    {
        auto c = m_panel.getColor(); c.a = clampToByte(m_panelAlpha);
        m_panel.setColor(c);
        m_panel.setPosition({ m_panelAnchor.x, m_panelAnchor.y + m_panelYOff });
    }
    if (m_npcAlpha > 0)
    {
        auto c = m_npc.getColor(); c.a = clampToByte(m_npcAlpha);
        m_npc.setColor(c);
        m_npc.setPosition({ m_npcAnchor.x, m_npcAnchor.y + m_npcYOff });
    }
    if (m_trainAlpha > 0)
    {
        auto c = m_train.getColor(); c.a = clampToByte(m_trainAlpha);
        m_train.setColor(c);
        m_train.setPosition({ m_trainAnchor.x, m_trainAnchor.y + m_trainYOff });
    }
}

void HintIcons::setPanelAnchor(sf::Vector2f pos) { m_panelAnchor = pos; }
void HintIcons::setNpcAnchor(sf::Vector2f pos) { m_npcAnchor = pos; }
void HintIcons::setTrainAnchor(sf::Vector2f pos) { m_trainAnchor = pos; }

void HintIcons::draw(sf::RenderWindow& window)
{
    if (m_panelAlpha > 0) window.draw(m_panel);
    if (m_npcAlpha > 0) window.draw(m_npc);
    if (m_trainAlpha > 0) window.draw(m_train);
}
