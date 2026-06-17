#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <headers/gameScreen.h>
#include <headers/dataStruct.h>
#include <headers/inputHandler.h>
#include <headers/gameState.h>
#include <headers/collision.h>
#include <cstdint>


class GameScreen 
{
public:

    GameScreen( const DataStruct& );

    eCollisionType CheckCollisions(const DataStruct &rTuple,const bool);
    void HandleCollisions(const DataStruct &rTuple, const bool, const eCollisionType, const int);
    void SetLastCollisionType(eCollisionType eJustHit){m_eLastCollisionType = eJustHit ;}
    eCollisionType GetLastCollisionType() const { return m_eLastCollisionType; }

    void SetSimFrameTopLastHit(uint64_t iFrame){m_iSimFrameTopLastHit = iFrame;}
    uint64_t GetSimFrameTopLastHit()const {return m_iSimFrameTopLastHit;}

    void ResetGame(const DataStruct&);
    void AttachBallToBat(std::shared_ptr<Bat> pBat, std::shared_ptr<Ball> pBall, const float);
    bool ShouldAttachBallToBat(const DataStruct&);

    bool GetShouldUpdateScore()const{ return m_bShouldUpdateScore; }
    void SetShouldUpdateScore(bool bShouldIt){ m_bShouldUpdateScore = bShouldIt; }

    bool isBallCollidingWithTarget(const sf::FloatRect box1, const sf::FloatRect box2);
    bool isBallHittingWall(const sf::FloatRect box1, const std::shared_ptr<sf::RenderWindow> pRenderWindow);
    bool isBallHittingGoal( const sf::FloatRect box1, const DataStruct &rTuple, const bool );

    std::string SetScoreText(const int &iPlayer1Score, const int &iPlayer2Score);

    sf::Vector2f CalculateScreenCenter(const std::shared_ptr<sf::RenderWindow> pRenderWindow);
    sf::Vector2f GetScreenCenter() const {return m_vScreenCenter;}
    void SetScreenCenter(sf::Vector2f fScreenCenter){m_vScreenCenter = fScreenCenter;} 

    void CreateGameScreen(const DataStruct& rTuple); 
    int UpdateGamescreen(const DataStruct& rTuple, sf::Clock &rTimer);

    void UpdateScoreText(const DataStruct& rTuple , int, bool);
    void UpdateHudText(const DataStruct& rTuple, std::string);
    bool UpdateUIText(const bool, const bool, const DataStruct& );

    sf::Clock GetGameClock() const { return m_hClock; }
    sf::Clock GetTextFadeTimer() const { return m_TextFadeOutTimer ;}

    void ShakeScreen(const DataStruct &rTuple, const float fMagnitude, const eCollisionType eJustHit, const bool );
    bool GetisWinConditionMet() {return (m_aScore[0] == m_iScoreLimit || m_aScore[1] == m_iScoreLimit); }

    void CreateMiddleLine(const DataStruct& rTuple);
    void DimMiddleLine(const DataStruct& rTuple,const bool bShouldDimLine);

    void SetBoundryEdgeShapes(const DataStruct& rTuple);
    void UpdateWallBounceVFX(const DataStruct& rTuple);

    float CreateRandomAngle(const int minRange, const int maxRange);



private:
    bool m_bShouldUpdateScore = true;
    const int m_iScoreLimit = 10;
    sf::Clock m_TextFadeOutTimer;
    eCollisionType m_eLastCollisionType;
    InputHandler m_hInputHandler;         
    sf::Vector2f m_vScreenCenter;
    sf::Clock m_hClock;
    uint8_t m_aScore[2] = {0};
    uint64_t m_iLastShakeFrame = 0;
    int32_t m_lLastGoalScoredFrame = -500;
    static const uint8_t m_iNumberOfLines = 6;
    sf::RectangleShape m_DashedLineRect[m_iNumberOfLines+1];

    uint64_t m_iSimFrameTopLastHit = 0;
    static constexpr int m_iBounceVFXArrayLength = 5;
    int pBounceWallVFXSpriteArray[m_iBounceVFXArrayLength];
    sf::RectangleShape m_sTopEdge[m_iBounceVFXArrayLength+1];
    sf::RectangleShape m_sBottomEdge[m_iBounceVFXArrayLength+1];
};

