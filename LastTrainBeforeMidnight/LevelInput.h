#pragma once
#include <SFML/Window/Keyboard.hpp>

class LevelInput
{
public:
    void update(); // lit l'état courant des touches et calcule les edges

    bool spaceDown() const { return m_space; }
    bool eDown()     const { return m_e; }
    bool spaceEdge() const { return m_space && !m_prevSpace; }
    bool eEdge()     const { return m_e && !m_prevE; }

private:
    bool m_prevSpace = false;
    bool m_prevE = false;
    bool m_space = false;
    bool m_e = false;
};
