#include <headers/gameState.h>


void GameState::DetermineGameState()
{
    const eGameState eCurrentGameState = GetCurrentGameState();
    const eGameState eDesiredGameState = GetDesiredGameState();
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
    else if ( eCurrentGameState == eGameState::GameOver )
    {
        if( eDesiredGameState == eGameState::Boot )
        {
            eNewGameState = eGameState::Boot;
        }
        else
        {
            eNewGameState = eGameState::Paused;
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
        else if( eDesiredGameState == eGameState::Paused )
        {
            eNewGameState = eGameState::Paused;
        }
    }
    if ( eNewGameState != eCurrentGameState )
    {
        SetCurrentGamestate(eNewGameState);
    }
    else
    {
        static_assert("Unhandled Transition");
    }
}

//----------------------------------------------------------