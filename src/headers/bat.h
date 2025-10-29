#pragma once
#include <SFML/Graphics.hpp>

enum class eBatMoveDirection
{
    NONE,
    UP,
    DOWN,
};

enum ePlayerNumber{
    PLAYER1,
    PLAYER2
};


class Bat
{

public:

    virtual ~Bat(){};

    Bat(sf::Vector2f, ePlayerNumber);
    Bat();

    void CalculateBatSpeed(sf::RenderWindow *pRenderWindow, float fLapsedTime);
    void NudgeBat(sf::RenderWindow* pRenderWindow);

    sf::Vector2f GetPosition() const { return m_vPosition ;}
    void SetPosition(sf::Vector2f _position) { m_vPosition = _position ; }

    sf::RectangleShape& GetRectReference(){ return m_hRectShape ;} 

    sf::RectangleShape GetShape() const { return m_hRectShape; }
    void SetShape( sf::RectangleShape _shape ) { m_hRectShape = _shape ;}

    float GetVelocity() const { return m_fVelocity ;}
    void SetVelocity(float _velocity ) { m_fVelocity = _velocity ;}

    float GetAccel() const {return m_fYaccel ;}
    float GetTopSpeed() const {return m_fYtopSpeed ;}

    ePlayerNumber GetPlayerNumber() const { return m_ePlayerNumber ;}

    void ModifyVelocity( float _speedMod ){ m_fVelocity += _speedMod ;}
    void DecayVelocity(){ m_fVelocity *= m_fYdecaySpeed ;}

    void UpdateTargetYPosition (float _fTargetYpos){ m_vPosition.y += _fTargetYpos;}

    void UpdateShapeToDesiredTransform(){m_hRectShape.setPosition(m_vPosition) ;}
    void UpdateDesiredToShapeTransform(){m_vPosition = m_hRectShape.getPosition() ;}

    bool IsWithinGameBounds(sf::Vector2u vRenderScreenArea)const{ return (m_vPosition.y < vRenderScreenArea.y + 20 && m_vPosition.y > 0.0f - 20) ;}
    bool IsHittingBottom(sf::Vector2u vRenderScreenArea) const {return m_vPosition.y >= vRenderScreenArea.y ;}
    bool IsHittingTop() const {return m_vPosition.y <= 0.0f ;}

    bool IsBouncing(float fSpeed);

    eBatMoveDirection DetermCurrentMoveDirection(sf::RenderWindow* pRenderWindow );

    eBatMoveDirection GetDesiredMoveDirection() const { return m_eDesiredMoveDirection; }
    void SetDesiredMoveDirection( eBatMoveDirection _direction ){ m_eDesiredMoveDirection = _direction ;}

    eBatMoveDirection GetCurrentMoveDirection() const { return m_eCurrentMoveDirection; }
    void SetCurrentMoveDirection( eBatMoveDirection _direction ){ m_eCurrentMoveDirection = _direction ;}


private:

    sf::Vector2f m_vPosition;
    sf::RectangleShape m_hRectShape;

    ePlayerNumber m_ePlayerNumber;

    eBatMoveDirection m_eDesiredMoveDirection = eBatMoveDirection::NONE;
    eBatMoveDirection m_eCurrentMoveDirection = eBatMoveDirection::NONE;

    const float m_fYaccel = 12.0f;
    const float m_fYtopSpeed = 12.00f;
    const float m_fYdecaySpeed = 0.995f;
    float m_fVelocity = 0.0f;
    float m_fSpeed = 0.0f;
};