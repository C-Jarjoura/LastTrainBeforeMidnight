#include "DialogueSystem.h"
#include <fstream>

static inline void setDialogPosition(sf::Text& t)
{
    // Centré en bas (1920x1080)
    auto b = t.getLocalBounds();
    t.setOrigin({ b.size.x * 0.5f, 0.f });
    t.setPosition({ 960.f, 930.f });
}

DialogueSystem::DialogueSystem(sf::Font& font)
    : m_text(font, "", 32)
{
    m_text.setFillColor(sf::Color::White);
    setDialogPosition(m_text);
}

std::vector<std::string> DialogueSystem::loadSceneDialog(int sceneId)
{
    std::vector<std::string> r;
    std::ifstream f("assets/dialogue/scene" + std::to_string(sceneId) + ".dlg");
    if (!f.is_open()) return r;

    std::string l;
    while (std::getline(f, l))
        if (!l.empty()) r.push_back(l);

    return r;
}

void DialogueSystem::centerDialog(sf::Text& t)
{
    setDialogPosition(t);
}

void DialogueSystem::startScene(int sceneId, sf::Vector2f /*playerPos*/, sf::Vector2f /*npcPos*/)
{
    m_lines = loadSceneDialog(sceneId);
    m_index = 0;

    if (m_lines.empty())
    {
        end();
        return;
    }

    m_active = true;
    m_text.setString(m_lines[m_index]);
    centerDialog(m_text);
}

void DialogueSystem::startWithLines(const std::vector<std::string>& lines)
{
    m_lines = lines;
    m_index = 0;

    if (m_lines.empty())
    {
        end();
        return;
    }

    m_active = true;
    m_text.setString(m_lines[m_index]);
    centerDialog(m_text);
}

void DialogueSystem::advance()
{
    if (!m_active) return;

    if (m_index + 1 < static_cast<int>(m_lines.size()))
    {
        ++m_index;
        m_text.setString(m_lines[m_index]);
        centerDialog(m_text);
    }
    else
    {
        end();
    }
}

void DialogueSystem::end()
{
    m_active = false;
    m_lines.clear();
    m_index = 0;
    m_text.setString("");
    centerDialog(m_text);
}

void DialogueSystem::draw(sf::RenderWindow& w)
{
    if (m_active)
        w.draw(m_text);
}
