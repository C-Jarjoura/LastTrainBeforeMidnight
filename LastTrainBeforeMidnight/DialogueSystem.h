#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class DialogueSystem
{
public:
    DialogueSystem(sf::Font& font);

    void startScene(int sceneId, const sf::Vector2f& playerPos, const sf::Vector2f& npcPos);
    void advance();
    void end();

    bool active() const { return m_inDialogue; }

    void draw(sf::RenderWindow& window);

private:
    std::vector<std::string> loadSceneDialog(int sceneId);
    void center();

private:
    sf::Text m_text;

    bool m_inDialogue = false;
    std::vector<std::string> m_lines;
    size_t m_index = 0;
};
