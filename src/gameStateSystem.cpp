#include <headers/gameStateSystem.h>


void GameStateSystem::ResetGame(const DataStruct &rTuple )
{

    rTuple.pBat1->SetPosition(sf::Vector2f(rTuple.g_fBAT_OFFSET , rTuple.fScreenHeight/2));
    rTuple.pBat1->SetDesiredMoveDirection(eBatMoveDirection::NONE);
    rTuple.pBat1->UpdateShapeToDesiredTransform();

    rTuple.pBat2->SetPosition(sf::Vector2f(rTuple.fScreenWidth - rTuple.g_fBAT_OFFSET , rTuple.fScreenHeight / 2 ));
    rTuple.pBat2->SetDesiredMoveDirection(eBatMoveDirection::NONE);
    rTuple.pBat2->UpdateShapeToDesiredTransform();

    rTuple.pBall->SetBallVector(sf::Vector3f(rTuple.fScreenWidth/2, rTuple.fScreenHeight/2, 0.00f));
    rTuple.pBall->StateMachine();

}

//----------------------------------------------------------


void GameStateSystem::DetermineGameState(const DataStruct &rTuple )
{
    const eGameState eCurrentGameState = rTuple.pCurrentWorldState -> GetCurrentGameState();
    const eGameState eDesiredGameState = rTuple.pDesiredWorldState -> GetDesiredGameState();
    eGameState eNewGameState = eCurrentGameState;

    if ( eDesiredGameState == eGameState::Quit)
    {
        eNewGameState = eGameState::Quit;
    }
    
    if(eCurrentGameState == eGameState::Boot)
    {
        if( eDesiredGameState == eGameState::Running )
        {
            eNewGameState = eGameState::Running;
        }
        else
        {
            eNewGameState = eGameState::Paused;
        }
    }
    else if(eCurrentGameState == eGameState::Reset)
    {
        eNewGameState = eGameState::Boot;
    }
    else if ( eCurrentGameState == eGameState::GameOver )
    {
        if( eDesiredGameState == eGameState::Reset )
        {
            eNewGameState = eGameState::Reset;
        }
    }
    else if ( eCurrentGameState == eGameState::Paused )
    {
        if( eDesiredGameState == eGameState::Running )
        {
            eNewGameState = eGameState::Running;
        }
    }
    else if ( eCurrentGameState == eGameState::Running )
    {
        if( eDesiredGameState == eGameState::GameOver )
        {
            eNewGameState = eGameState::GameOver;
        }
        else if( eDesiredGameState == eGameState::Reset )
        {
            if( eDesiredGameState == eGameState::Boot )
            {
                eNewGameState = eGameState::Boot;
            }
        }
        else if( eDesiredGameState == eGameState::Paused )
        {
            eNewGameState = eGameState::Paused;
        }
    }
    if ( eNewGameState != eCurrentGameState )
    {
        rTuple.pCurrentWorldState->SetCurrentGamestate(eNewGameState);
    }
    else
    {
        static_assert("Unhandled Transition");
    }
}

//----------------------------------------------------------