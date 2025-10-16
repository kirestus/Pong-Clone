#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <headers/bootscreen.h>
#include <headers/bat.h>
#include <headers/tuple.h>

class BootScreen 
{
    public:

        void DetermineEvent(sf::RenderWindow *pRenderWindow);

        BootScreen( tuple );

        sf::CircleShape CreateCircleShape(BootScreen *pBootScreen, float fradius);

        sf::Vector2f CalculateScreenCenter(sf::RenderWindow* pRenderWindow);
        sf::Vector2f GetScreenCenter() const {return m_vScreenCenter;}
        void SetScreenCenter(sf::Vector2f fScreenCenter){m_vScreenCenter = fScreenCenter;}

        sf::CircleShape GetCircleShape() const {return m_sCircleShape;}
        void SetCircleShape(sf::CircleShape pCircle){m_sCircleShape = pCircle;}

        float GetYPos()const {return GetCircleShape().getPosition().y;}

        void CreateBootScreen(tuple rTuple);
        int UpdateBootscreen(tuple rTuple, sf::Clock &rTimer);

        sf::Clock GetGameClock() const {return m_hClock;}


    private:         
        sf::Vector2f m_vScreenCenter;
        sf::CircleShape m_sCircleShape;
        sf::Clock m_hClock;
        
        sf::Vector2f m_vPosition;

        eBatMoveDirection m_eDesiredMoveDirection = eBatMoveDirection::NONE;
        const float m_fYaccel = 0.55f;
        const float m_fYtopSpeed = 5.00f;
        const float m_fYdecaySpeed = 0.9994f;
        float m_fTimeElapsed = 0.00f;
        float m_fVelocity = 0.0f;
};

