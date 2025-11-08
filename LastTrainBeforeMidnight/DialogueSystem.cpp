#include "DialogueSystem.h"
#include <fstream>

DialogueSystem::DialogueSystem(sf::Font& font)
    : m_text(font, "", 32)
{
    m_text.setFillColor(sf::Color::White);
}

std::vector<std::string> DialogueSystem::loadSceneDialog(int sceneId)
{
    std::vector<std::string> r;
    std::ifstream f("assets/dialogue/scene" + std::to_string(sceneId) + ".dlg");
    if (!f.is_open()) return r;
    std::string line;
    while (std::getline(f, line))
        if (!line.empty()) r.push_back(line);
    return r;
}

void DialogueSystem::center()
{
    auto b = m_text.getLocalBounds();
    m_text.setOrigin({ b.size.x * 0.5f, 0.f });
    m_text.setPosition({ 960.f,930.f });
}

void DialogueSystem::startScene(int sceneId, const sf::Vector2f& playerPos, const sf::Vector2f& npcPos)
{
    m_lines = loadSceneDialog(sceneId);
    m_index = 0;

    if (m_lines.empty())
    {
        m_inDialogue = false;
        return;
    }

    m_inDialogue = true;
    m_text.setString(m_lines[m_index]);
    center();
}

void DialogueSystem::advance()
{
    if (!m_inDialogue) return;

    if (m_index + 1 < m_lines.size())
    {
        ++m_index;
        m_text.setString(m_lines[m_index]);
        center();
    }
    else
    {
        end();
    }
}

void DialogueSystem::end()
{
    m_inDialogue = false;
    m_lines.clear();
    m_text.setString("");
}

void DialogueSystem::draw(sf::RenderWindow& window)
{
    if (m_inDialogue)
        window.draw(m_text);
}
