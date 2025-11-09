#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class DialogueSystem
{
public:
    explicit DialogueSystem(sf::Font& font);

    bool active() const { return m_active; }

    void startScene(int sceneId, sf::Vector2f playerPos, sf::Vector2f npcPos);
    void startWithLines(const std::vector<std::string>& lines);

    void advance();
    void end();

    void draw(sf::RenderWindow& w);

private:
    static std::vector<std::string> loadSceneDialog(int sceneId);
    static void centerDialog(sf::Text& t);

private:
    sf::Text m_text;
    std::vector<std::string> m_lines;
    int  m_index = 0;
    bool m_active = false;
};
