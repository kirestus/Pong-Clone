#pragma once
#include <SFML/Graphics.hpp>
enum class eBatMoveDirection
{
    NONE,
    UP,
    DOWN,
};

class Bat
{
    public:
    Bat(sf::Vector2f vPosition);
    Bat();

    sf::Vector2f GetPosition() const { return m_vPosition ;}
    void SetPosition(sf::Vector2f _position) { m_vPosition = _position ; }

    sf::RectangleShape& GetRectReference(){ return m_hRectShape ;} 

    sf::RectangleShape GetShape() const { return m_hRectShape; }
    void SetShape( sf::RectangleShape _shape ) { m_hRectShape = _shape ;}

    float GetVelocity() const { return m_fVelocity ;}
    void SetVelocity(float _velocity ) { m_fVelocity = _velocity ;}

    float GetAccel() const {return m_fYaccel ;}
    float GetTopSpeed() const {return m_fYtopSpeed ;}

    void ModifyVelocity( float _speedMod ){m_fVelocity += _speedMod ;}
    void DecayVelocity(){ m_fVelocity *= m_fYdecaySpeed ;}

    void UpdateTargetYPosition (float _fTargetYpos){ m_vPosition.y += _fTargetYpos;}

    void UpdateShapeToDesiredTransform(){m_hRectShape.setPosition(m_vPosition) ;}
    void UpdateDesiredToShapeTransform(){m_vPosition = m_hRectShape.getPosition() ;}

    bool IsWithinGameBounds(sf::Vector2u vRenderScreenArea)const{ return (m_vPosition.y < vRenderScreenArea.y && m_vPosition.y > 0.0f) ;}
    bool IsHittingBottom(sf::Vector2u vRenderScreenArea) const {return m_vPosition.y > vRenderScreenArea.y ;}
    bool IsHittingTop() const {return m_vPosition.y < 0.0f ;}

    eBatMoveDirection GetMovementDirection() const { return m_eDesiredMoveDirection; }
    void SetMovementDirection( eBatMoveDirection _direction ){ m_eDesiredMoveDirection = _direction ;}



    private:
        sf::Vector2f m_vPosition;
        sf::RectangleShape m_hRectShape;

        eBatMoveDirection m_eDesiredMoveDirection = eBatMoveDirection::NONE;
        const float m_fYaccel = 12.0f;
        const float m_fYtopSpeed = 20.00f;
        const float m_fYdecaySpeed = 0.999f;
        float m_fVelocity = 0.0f;
};