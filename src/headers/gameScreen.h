#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <headers/gameScreen.h>
#include <headers/dataStruct.h>
#include <headers/inputHandler.h>
#include <headers/gameState.h>

class GameScreen 
{
    public:

        GameScreen( DataStruct& );
        virtual ~GameScreen();

        void ResetGame(DataStruct &rTuple);
        void CheckCollisions(DataStruct &rTuple, const bool);

        bool isBallCollidingWithTarget(const sf::FloatRect box1, const sf::FloatRect box2);
        bool isBallHittingWall(const sf::FloatRect box1, sf::RenderWindow* pRenderWindow);
        bool isBallHittingGoal( const sf::FloatRect box1, DataStruct &rTuple);

        std::string SetScoreText(int &iPlayer1Score, int &iPlayer2Score);

        sf::Vector2f CalculateScreenCenter(sf::RenderWindow* pRenderWindow);
        sf::Vector2f GetScreenCenter() const {return m_vScreenCenter;}
        void SetScreenCenter(sf::Vector2f fScreenCenter){m_vScreenCenter = fScreenCenter;} 

        void CreateGameScreen(DataStruct& rTuple); 
        int UpdateGamescreen(DataStruct& rTuple, sf::Clock &rTimer);

        void UpdateScoreText(DataStruct& rTuple);

        sf::Clock GetGameClock() const { return m_hClock; }

        GameState* GetGameState() const { return m_pGameScreenGameState ;}

    private:
        GameState* m_pGameScreenGameState = nullptr;
        InputHandler m_hInputHandler;         
        sf::Vector2f m_vScreenCenter;
        sf::Clock m_hClock;
        int m_aScore[2] = {0};
};

