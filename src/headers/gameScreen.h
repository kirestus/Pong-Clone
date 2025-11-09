#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <headers/gameScreen.h>
#include <headers/dataStruct.h>
#include <headers/inputHandler.h>
#include <headers/gameState.h>
#include <headers/collision.h>


class GameScreen 
{
public:

    GameScreen( DataStruct& );

    eCollisionType CheckCollisions(DataStruct &rTuple);
    void HandleCollisions(DataStruct &rTuple, const bool, const eCollisionType);

    void ResetGame(DataStruct&);
    void AttachBallToBat(std::shared_ptr<Bat> pBat, std::shared_ptr<Ball> pBall);

    bool GetShouldUpdateScore()const{ return m_bShouldUpdateScore; }
    void SetShouldUpdateScore(bool bShouldIt){ m_bShouldUpdateScore = bShouldIt; }

    bool isBallCollidingWithTarget(const sf::FloatRect box1, const sf::FloatRect box2);
    bool isBallHittingWall(const sf::FloatRect box1, const std::shared_ptr<sf::RenderWindow> pRenderWindow);
    bool isBallHittingGoal( const sf::FloatRect box1, DataStruct &rTuple);

    std::string SetScoreText(const int &iPlayer1Score, const int &iPlayer2Score);

    sf::Vector2f CalculateScreenCenter(const std::shared_ptr<sf::RenderWindow> pRenderWindow);
    sf::Vector2f GetScreenCenter() const {return m_vScreenCenter;}
    void SetScreenCenter(sf::Vector2f fScreenCenter){m_vScreenCenter = fScreenCenter;} 

    void CreateGameScreen(DataStruct& rTuple); 
    int UpdateGamescreen(DataStruct& rTuple, sf::Clock &rTimer);

    void UpdateScoreText(DataStruct& rTuple);
    void UpdateScoreText(DataStruct& rTuple, std::string);
    void UpdateUIText(bool, DataStruct& );

    sf::Clock GetGameClock() const { return m_hClock; }

    bool GetisWinConditionMet() {return (m_aScore[0] == m_iScoreLimit || m_aScore[1] == m_iScoreLimit); }

private:
    bool m_bShouldUpdateScore = true;
    const int m_iScoreLimit = 10;
    InputHandler m_hInputHandler;         
    sf::Vector2f m_vScreenCenter;
    sf::Clock m_hClock;
    int m_aScore[2] = {0};
};

