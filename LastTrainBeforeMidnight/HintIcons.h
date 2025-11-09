#pragma once
#include <SFML/Graphics.hpp>

class HintIcons
{
public:
    HintIcons(sf::Sprite& panelIcon, sf::Sprite& npcIcon, sf::Sprite& trainIcon);

    // Mise à jour des alpha (fade) selon visibilité
    void update(float dt, bool showPanel, bool showNpc, bool showTrain);

    // Ancrages cibles pour positionner les icônes
    void setPanelAnchor(sf::Vector2f pos); // milieu du panneau
    void setNpcAnchor(sf::Vector2f pos);   // position du NPC
    void setTrainAnchor(sf::Vector2f pos); // milieu de la zone train

    // Dessin
    void draw(sf::RenderWindow& window);

    // Constantes réglables
    void setFadeSpeed(float s) { m_fadeSpeed = s; }
    void setOffsets(float panelY, float npcY, float trainY)
    {
        m_panelYOff = panelY; m_npcYOff = npcY; m_trainYOff = trainY;
    }

private:
    // références vers les sprites (appartenant à Level)
    sf::Sprite& m_panel;
    sf::Sprite& m_npc;
    sf::Sprite& m_train;

    // états de fade (0..255)
    int m_panelAlpha = 0;
    int m_npcAlpha = 0;
    int m_trainAlpha = 0;

    // ancrages (actualisés chaque frame si visibles)
    sf::Vector2f m_panelAnchor{ 0.f, 0.f };
    sf::Vector2f m_npcAnchor{ 0.f, 0.f };
    sf::Vector2f m_trainAnchor{ 0.f, 0.f };

    // réglages
    float m_fadeSpeed = 30.f;
    float m_panelYOff = 0.f;
    float m_npcYOff = -40.f;   // au-dessus de la tête par défaut
    float m_trainYOff = -40.f;

private:
    static inline int approachAlpha(int current, int target, float k);
};
