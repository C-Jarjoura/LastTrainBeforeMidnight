#pragma once
#include <SFML/Graphics.hpp>

struct PanelState
{
    bool hasPrev = false;
    bool hasNext = false;
    sf::Sprite* panelPrev = nullptr;
    sf::Sprite* panelNext = nullptr;
    sf::Rect<float> zonePrev;
    sf::Rect<float> zoneNext;
};

struct TrainState
{
    bool hasTrain = false;
    sf::Rect<float> zoneTrain;
};

class SceneNavigation
{
public:
    // calcule les flags d’UI (“sur un panneau ?”, “sur le train ?”)
    // et renvoie les booléens onPrev/onNext/showPanel/showTrain
    void compute(const sf::Rect<float>& playerRect,
        const PanelState& panels,
        const TrainState& train,
        bool& showPanel,
        bool& onPrev,
        bool& onNext,
        bool& showTrain) const;

    // décide du changement de scène en fonction de l’input E
    // renvoie -1 si aucun changement, sinon l’id de destination
    int decideSceneChange(bool eEdge,
        bool onPrev,
        bool onNext,
        bool showTrain,
        int sceneId,
        int winningStation) const;

    // utilitaire
    static bool aabbOverlap(const sf::Rect<float>& a, const sf::Rect<float>& b);
};
