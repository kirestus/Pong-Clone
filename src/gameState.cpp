#include <headers/gameState.h>


void GameState::DetermineGameState()
{
    eGameState eCurrentGameState = GetCurrentGameState();
    eGameState eDesiredGameState = GetDesiredGameState();
    eGameState eNewGameState = eCurrentGameState;

    if ( eDesiredGameState == eGameState::Quit)
    {
        eNewGameState = Quit;
    }
    
    if(eCurrentGameState == eGameState::Boot)
    {
        if( eDesiredGameState == eGameState::Running )
        {
            eNewGameState = Running;
        }
        else
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
        else
        {
            eNewGameState = Paused;
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