#pragma once
#include <SFML/Graphics.hpp>
#include <headers/alias.h>

enum class eGameState : uint8
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

    int64 GetCurrentSimFrame()const {return m_iDetermFrame; }
    void IncrimentSimFrame(){m_iDetermFrame ++;}

    bool GetDidBallLastHitScreenTop()const {return m_bBallLastHitScreenTop ;}//TODO: move this into the ball class
    void SetBallLastHitTop(bool hitTop){m_bBallLastHitScreenTop = hitTop ;}

    bool IsScreenShaking()const {return m_bIsScreenShaking ;}
    void SetIsScreenShaking(bool isShaking){m_bIsScreenShaking = isShaking;}

    int64 GetSimFrameOfLastScoredGoal() const {return m_iSimFrameOfLastScoredGoal;}
    void SetSimFrameOfLastScoredGoal(int64 iFrame){m_iSimFrameOfLastScoredGoal = iFrame;}
    bool GetisWinConditionMet() const {return (m_aScore[0] == m_iScoreLimit || m_aScore[1] == m_iScoreLimit); }
    const std::array<uint8, 2>& GetScore() const {return m_aScore;}

    void SetSimFrameTopLastHit(uint64 iFrame){m_iSimFrameTopLastHit = iFrame;}
    uint64 GetSimFrameTopLastHit()const {return m_iSimFrameTopLastHit;}


private:

    eGameState m_eCurrentGameState;
    eGameState m_eDesiredGameState = eGameState::Boot;

    int64 m_iDetermFrame = 0;
    int64 m_iSimFrameOfLastScoredGoal = 0;

    static const uint8 m_iScoreLimit = 10;
    std::array<uint8, 2> m_aScore = {0, 0};
    uint16 m_iSimFrameTopLastHit = 0;

    bool m_bIsScreenShaking = false;
    bool m_bBallLastHitScreenTop = false;
    bool m_bLastGoalScoredOnP1 = false;
    bool m_bShouldPlaySFX = true;
};