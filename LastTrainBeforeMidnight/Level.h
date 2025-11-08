#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include "Player.h"

// Level est "aveugle" du Game : il expose des callbacks.
class Level
{
public:
    Level();

    // Game appelle ceci quand la scène change
    void setScene(int sceneId, const sf::Texture& sceneTexture);

    void update(float dt);
    void draw(sf::RenderWindow& window);

    // callbacks (branchés par Game)
    std::function<void()> onNextStation; // scène suivante
    std::function<void()> onPrevStation; // scène précédente

private:
    // configuration des panneaux selon la scène (positions / visibles)
    void configureForScene(int sceneId);

    // utilitaire AABB (Rect<float> SFML3 : position/size)
    static bool aabbOverlap(const sf::Rect<float>& a, const sf::Rect<float>& b);

private:
    // --- fond de scène (Level ne possède pas la texture, il en garde un pointeur) ---
    const sf::Texture* m_sceneTexPtr = nullptr; // fixé par setScene

    // --- panneau(s) & texte ---
    sf::Texture m_panelTex;       // "assets/sprites/panel.png"
    sf::Sprite  m_panelNext;      // panneau "aller"
    sf::Sprite  m_panelPrev;      // panneau "retour"

    bool m_hasNext = false;
    bool m_hasPrev = false;

    sf::Rect<float> m_zoneNext{ {0.f,0.f}, {0.f,0.f} };
    sf::Rect<float> m_zonePrev{ {0.f,0.f}, {0.f,0.f} };

    sf::Font m_font;              // "assets/fonts/arial.ttf"
    sf::Text m_pressE;            // "Press E" (apparait au survol)

    // --- joueur ---
    Player m_player;
};
