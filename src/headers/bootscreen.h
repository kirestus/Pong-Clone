#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <headers/bootscreen.h>
#include <headers/tuple.h>
#include <headers/inputHandler.h>

class BootScreen 
{
    public:

        BootScreen( tuple& );

        sf::Vector2f CalculateScreenCenter(sf::RenderWindow* pRenderWindow);
        sf::Vector2f GetScreenCenter() const {return m_vScreenCenter;}
        void SetScreenCenter(sf::Vector2f fScreenCenter){m_vScreenCenter = fScreenCenter;} 

        void CreateBootScreen(tuple& rTuple); 
        int UpdateBootscreen(tuple& rTuple, sf::Clock &rTimer);

        sf::Clock GetGameClock() const {return m_hClock;}

    private:
        InputHandler m_hInputHandler;         
        sf::Vector2f m_vScreenCenter;
        sf::Clock m_hClock;
        float m_fTimeElapsed = 0.00f;

};

