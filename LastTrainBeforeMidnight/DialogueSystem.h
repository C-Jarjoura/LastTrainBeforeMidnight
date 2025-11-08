#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class DialogueSystem
{
public:
    explicit DialogueSystem(sf::Font& font);

    // État
    bool active() const { return m_active; }

    // Démarrages possibles
    void startScene(int sceneId, sf::Vector2f playerPos, sf::Vector2f npcPos); // compat
    void startWithLines(const std::vector<std::string>& lines);                // nouveau

    // Avancer/terminer
    void advance();
    void end();

    // Rendu
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
