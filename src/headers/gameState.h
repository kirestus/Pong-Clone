#pragma once
enum eGameState
{
    Boot,
    Paused,
    GameOver,
    Running,
    Quit
};

class GameState{

public:
    void DetermineGameState();

    eGameState GetCurrentGameState() const {return m_eCurrentGameState ;}
    void SetCurrentGamestate( eGameState _gamestate ){ m_eCurrentGameState = _gamestate ;}

    eGameState GetDesiredGameState() const {return m_eDesiredGameState ;}
    void SetDesiredGamestate( eGameState _gamestate ){ m_eDesiredGameState = _gamestate ;}
private:

    eGameState m_eCurrentGameState;
    eGameState m_eDesiredGameState = eGameState::Boot;
};