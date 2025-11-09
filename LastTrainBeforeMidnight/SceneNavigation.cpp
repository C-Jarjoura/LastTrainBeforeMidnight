#include "SceneNavigation.h"

bool SceneNavigation::aabbOverlap(const sf::Rect<float>& a, const sf::Rect<float>& b)
{
    return (a.position.x < b.position.x + b.size.x) &&
        (a.position.x + a.size.x > b.position.x) &&
        (a.position.y < b.position.y + b.size.y) &&
        (a.position.y + a.size.y > b.position.y);
}

void SceneNavigation::compute(const sf::Rect<float>& playerRect,
    const PanelState& panels,
    const TrainState& train,
    bool& showPanel,
    bool& onPrev,
    bool& onNext,
    bool& showTrain) const
{
    showPanel = false;
    onPrev = false;
    onNext = false;

    if (panels.hasNext && aabbOverlap(playerRect, panels.zoneNext))
    {
        showPanel = true; onNext = true;
    }
    else if (panels.hasPrev && aabbOverlap(playerRect, panels.zonePrev))
    {
        showPanel = true; onPrev = true;
    }

    showTrain = (train.hasTrain && aabbOverlap(playerRect, train.zoneTrain));
}

int SceneNavigation::decideSceneChange(bool eEdge,
    bool onPrev,
    bool onNext,
    bool showTrain,
    int sceneId,
    int winningStation) const
{
    if (!eEdge) return -1;

    if (onNext) return sceneId + 1;
    if (onPrev) return sceneId - 1;

    if (showTrain)
    {
        // logique train : si station gagnante -> 99, sinon -> 1
        return (sceneId == winningStation) ? 99 : 1;
    }

    return -1;
}
