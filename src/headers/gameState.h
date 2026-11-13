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


protected:
    int64 m_iDetermFrame = 0;
    int64 m_iSimFrameOfLastScoredGoal = 0;
};

class DesiredGameState : public GameState
{
    public:
    eGameState GetDesiredGameState() const {return m_eDesiredGameState ;}
    void SetDesiredGamestate( eGameState _gamestate ){ m_eDesiredGameState = _gamestate ;}

    private:
    eGameState m_eDesiredGameState = eGameState::Boot;
};

class CurrentGameState : public GameState
{
    public:
    eGameState GetCurrentGameState() const {return m_eCurrentGameState ;}
    void SetCurrentGamestate( eGameState _gamestate ){ m_eCurrentGameState = _gamestate ;}

    private:
    eGameState m_eCurrentGameState;
};

class cogGameState : public GameState
{
    public:
    //void DetermineGameState();

     bool GetLastGoalScoredOnP1()const{ return m_bLastGoalScoredOnP1 ;}
     void SetLastGoalScoredOnP1( bool bIsScoredOnP1 ){ m_bLastGoalScoredOnP1 =  bIsScoredOnP1 ;}

     bool GetShouldPlaySFX()const{ return m_bShouldPlaySFX; }
     void SetShouldPlaySFX(bool bShouldPlay) { m_bShouldPlaySFX = bShouldPlay; }

     int64 GetCurrentSimFrame()const {return m_iDetermFrame; }
     void IncrimentSimFrame(){m_iDetermFrame ++;}

     bool GetDidBallLastHitScreenTop()const {return m_bBallLastHitScreenTop ;}
     void SetBallLastHitTop(bool hitTop){m_bBallLastHitScreenTop = hitTop ;}

     bool IsScreenShaking()const {return m_bIsScreenShaking ;}
     void SetIsScreenShaking(bool isShaking){m_bIsScreenShaking = isShaking;}

     int64 GetSimFrameOfLastScoredGoal() const {return m_iSimFrameOfLastScoredGoal;}
     void SetSimFrameOfLastScoredGoal(int64 iFrame){m_iSimFrameOfLastScoredGoal = iFrame;}
     bool GetisWinConditionMet() const {return (m_aScore[0] >= m_iScoreLimit || m_aScore[1] >= m_iScoreLimit); }
    
     const uint8* GetScore() const { return m_aScore ;}
     void SetScore(uint8 p1Score, uint8 p2Score){m_aScore[0]=p1Score, m_aScore[1] = p2Score ;}

     void SetSimFrameWallWasHit(uint64 iFrame){m_iSimeFrameWallLastHit = iFrame;}
     uint64 GetSimFrameWallWasHit()const {return m_iSimeFrameWallLastHit;}


    private:
        static const uint8 m_iScoreLimit = 10;
        uint8 m_aScore[2] = {0,0};
        uint16 m_iSimeFrameWallLastHit = 0;

        bool m_bIsScreenShaking = false;
        bool m_bBallLastHitScreenTop = false;
        bool m_bLastGoalScoredOnP1 = false;
        bool m_bShouldPlaySFX = true;
};