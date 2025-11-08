#include "DialogueSystem.h"
#include <fstream>

static inline void centerDialog(sf::Text& t)
{
    auto b = t.getLocalBounds();
    t.setOrigin({ b.size.x * 0.5f, 0.f });
    t.setPosition({ 960.f, 930.f });
}

DialogueSystem::DialogueSystem(sf::Font& font)
    : m_font(font)
    , m_text(font, "", 32)
{
    m_text.setFillColor(sf::Color::White);
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

void DialogueSystem::startScene(int sceneId, sf::Vector2f, sf::Vector2f)
{
    m_lines = loadSceneDialog(sceneId);
    m_index = 0;
    if (m_lines.empty()) { end(); return; }

    m_active = true;
    m_text.setString(m_lines[m_index]);
    centerDialog(m_text);
}

void DialogueSystem::advance()
{
    if (!m_active) return;

    if (m_index + 1 < (int)m_lines.size())
    {
        ++m_index;
        m_text.setString(m_lines[m_index]);
        centerDialog(m_text);
    }
    else end();
}

void DialogueSystem::draw(sf::RenderWindow& w)
{
    if (m_active) w.draw(m_text);
}

void DialogueSystem::end()
{
    m_active = false;
    m_lines.clear();
    m_index = 0;
    m_text.setString("");
}
