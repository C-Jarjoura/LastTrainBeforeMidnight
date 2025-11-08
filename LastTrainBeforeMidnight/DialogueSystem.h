#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class DialogueSystem
{
public:
    DialogueSystem(sf::Font& font);

    void startScene(int sceneId, sf::Vector2f playerPos, sf::Vector2f npcPos);
    void advance();
    void draw(sf::RenderWindow& w);
    void end();

    bool active() const { return m_active; }

private:
    std::vector<std::string> loadSceneDialog(int sceneId);  // <--- manquait ici !!!

private:
    sf::Font& m_font;
    sf::Text  m_text;

    std::vector<std::string> m_lines;
    int  m_index = 0;
    bool m_active = false;
};
