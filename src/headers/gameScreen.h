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

        sf::Vector2f CalculateScreenCenter(sf::RenderWindow* pRenderWindow);
        sf::Vector2f GetScreenCenter() const {return m_vScreenCenter;}
        void SetScreenCenter(sf::Vector2f fScreenCenter){m_vScreenCenter = fScreenCenter;} 
        sf::String DebugTextGameState( eGameState eGameState );

        void CreateGameScreen(DataStruct& rTuple); 
        int UpdateGamescreen(DataStruct& rTuple, sf::Clock &rTimer);

        sf::Clock GetGameClock() const { return m_hClock; }

        GameState* GetGameState() const { return m_pGameScreenGameState ;}

    private:
        GameState* m_pGameScreenGameState = nullptr;
        InputHandler m_hInputHandler;         
        sf::Vector2f m_vScreenCenter;
        sf::Clock m_hClock;
        float m_fTimeElapsed = 0.00f;
};

