#include <headers/gameStateSystem.h>


void GameStateSystem::ResetGame(const DataStruct &rTuple)
{
    rTuple.pBat1->SetPosition(sf::Vector2f(50.0f, rTuple.fScreenHeight/2));
    rTuple.pBat1->SetDesiredMoveDirection(eBatMoveDirection::NONE);
    rTuple.pBat1->UpdateShapeToDesiredTransform();

    rTuple.pBat2->SetPosition(sf::Vector2f(rTuple.fScreenWidth -50.0f, rTuple.fScreenHeight / 2 ));
    rTuple.pBat2->SetDesiredMoveDirection(eBatMoveDirection::NONE);
    rTuple.pBat2->UpdateShapeToDesiredTransform();

    rTuple.pBall->SetBallVector(sf::Vector3f(rTuple.fScreenWidth/2, rTuple.fScreenHeight/2, 0.00f));
    rTuple.pBall->StateMachine(rTuple.fScreenWidth);
}

//----------------------------------------------------------