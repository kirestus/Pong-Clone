#include <headers/gameState.h>


void GameState::DetermineGameState()
{
    eGameState eCurrentGameState = GetCurrentGameState();
    eGameState eDesiredGameState = GetDesiredGameState();
    eGameState eNewGameState = eCurrentGameState;

    if(eCurrentGameState == eGameState::Boot)
    {
        if( eDesiredGameState == eGameState::Running )
        {
            eNewGameState = Running;
        }
        else if( eDesiredGameState == eGameState::Paused )
        {
            eNewGameState = Paused;
        }
    }
    else if ( eCurrentGameState == eGameState::GameOver )
    {
        if( eDesiredGameState == eGameState::Boot )
        {
            eNewGameState = Boot;
        }
    }
    else if ( eCurrentGameState == eGameState::Paused )
    {
        if( eDesiredGameState == eGameState::Running )
        {
            eNewGameState = Running;
        }
    }
    else if ( eCurrentGameState == eGameState::Running )
    {
        if( eDesiredGameState == eGameState::GameOver )
        {
            eNewGameState = GameOver;
        }
        else if( eDesiredGameState == eGameState::Paused )
        {
            eNewGameState = Paused;
        }
    }

    if ( eNewGameState != eCurrentGameState )
    {
        SetCurrentGamestate(eNewGameState);
    }
}