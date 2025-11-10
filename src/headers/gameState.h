#pragma once
#include <SFML/Graphics.hpp>
enum eGameState
{
    Boot,
    Paused,
    GameOver,
    Running,
    Reset,
    Quit
};

class GameState{

public:
    void DetermineGameState();

    eGameState GetCurrentGameState() const {return m_eCurrentGameState ;}
    void SetCurrentGamestate( eGameState _gamestate ){ m_eCurrentGameState = _gamestate ;}

    eGameState GetDesiredGameState() const {return m_eDesiredGameState ;}
    void SetDesiredGamestate( eGameState _gamestate ){ m_eDesiredGameState = _gamestate ;}

    bool GetLastGoalScoredOnP1()const{ return m_bLastGoalScoredOnP1 ;}
    void SetLastGoalScoredOnP1( bool bIsScoredOnP1 ){ m_bLastGoalScoredOnP1 =  bIsScoredOnP1 ;}
private:

    eGameState m_eCurrentGameState;
    eGameState m_eDesiredGameState = eGameState::Boot;

    bool m_bLastGoalScoredOnP1 = false;
};