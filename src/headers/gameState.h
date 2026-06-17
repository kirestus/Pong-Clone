#pragma once
#include <SFML/Graphics.hpp>
enum class eGameState : uint8_t
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

    bool GetShouldPlaySFX()const{ return m_bShouldPlaySFX; }
    void SetShouldPlaySFX(bool bShouldPlay) { m_bShouldPlaySFX = bShouldPlay; }

    int GetCurrentSimFrame()const {return m_iDetermFrame; }
    void IncrimentSimFrame(){m_iDetermFrame ++;}

    bool GetDidBallLastHitScreenTop()const {return m_bBallLastHitScreenTop ;}
    void SetBallLastHitTop(bool hitTop){m_bBallLastHitScreenTop = hitTop ;}

    bool IsScreenShaking()const {return m_bIsScreenShaking ;}
    void SetIsScreenShaking(bool isShaking){m_bIsScreenShaking = isShaking;}

private:

    eGameState m_eCurrentGameState;
    eGameState m_eDesiredGameState = eGameState::Boot;

    int m_iDetermFrame = 0;

    bool m_bIsScreenShaking = false;
    bool m_bBallLastHitScreenTop = false;
    bool m_bLastGoalScoredOnP1 = false;
    bool m_bShouldPlaySFX = true;
};