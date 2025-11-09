#pragma once
#include <SFML/Graphics.hpp>

class MainMenu
{
public:
    MainMenu();

    // returns:
    // -1 = nothing
    //  1 = jouer
    //  2 = quitter
    int update(float dt);

    void draw(sf::RenderWindow& window);

private:
    sf::Texture m_bgTex;
    sf::Sprite  m_bgSprite;

    sf::RectangleShape m_highlight;
    int m_selected = 0;

    void updateHighlight();
};
    