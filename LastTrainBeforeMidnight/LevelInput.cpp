#include "LevelInput.h"

void LevelInput::update()
{
    // mémoriser précédent
    m_prevSpace = m_space;
    m_prevE = m_e;

    // lire courant
    m_space = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space);
    m_e = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::E);
}
