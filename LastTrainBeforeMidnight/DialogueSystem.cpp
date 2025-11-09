#include "DialogueSystem.h"
#include <fstream>
#include <random>
#include "SoundBank.h"

// petit RNG global
static std::mt19937 rng{ std::random_device{}() };

extern SoundBank* gSound;

// ----------------------------------------
static inline void setDialogPosition(sf::Text& t)
{
    // Centré en bas (1920x1080)
    auto b = t.getLocalBounds();
    t.setOrigin({ b.size.x * 0.5f, 0.f });
    t.setPosition({ 960.f, 930.f });
}

static void playBleep()
{
    if (!gSound) return;

    // random pitch (undertale like)
    std::uniform_real_distribution<float> dp(0.93f, 1.06f);
    float pitch = dp(rng);

    // random sample 0..2
    std::uniform_int_distribution<int> di(0, 2);
    int i = di(rng);

    const char* id = (i == 0) ? "blipA" : (i == 1) ? "blipB" : "blipC";

    // volume autour 63% (assez prononcé mais pas trop)
    // SoundBank ne supporte pas pitch, mais SFML3 le supporte sur sf::Sound
    // → on doit appeler playOneShot puis modifier le dernier
    // solution simple: juste jouer le sample (pitch empilé) :
    gSound->playOneShot(id, 63.f);

    // NOTE:
    // SFML3 n'expose pas pitch par SoundBank pour oneshots
    // mais on peut le faire si plus tard tu veux gérer un pool dédié aux bleeps
}

// ----------------------------------------
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
    playBleep();
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
    playBleep();
}

void DialogueSystem::advance()
{
    if (!m_active) return;

    if (m_index + 1 < static_cast<int>(m_lines.size()))
    {
        ++m_index;
        m_text.setString(m_lines[m_index]);
        centerDialog(m_text);
        playBleep();
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
