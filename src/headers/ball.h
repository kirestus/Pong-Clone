#pragma once
#include <SFML/Graphics.hpp>

enum eBallState{
    None,
    LEFT,
    RIGHT,
    AtPlayer1,
    AtPlayer2,
    GoalOnPlayer1,
    GoalOnPlayer2,
    ResetGamePosition,
};

class Ball
{
    public:
        Ball(sf::RenderWindow*);

        sf::RectangleShape& ReferenceShape(){return m_sShape ;}

        sf::RectangleShape GetShape() const { return m_sShape; }
        void SetShape( sf::RectangleShape _shape ) { m_sShape = _shape ;}

        float GetSpeed() const { return m_fSpeed ;}
        void SetSpeed( float _speed) { m_fSpeed = _speed ;}

        sf::Vector2f GetTranslationPosition() const {return sf::Vector2f(m_vBallVector.x, m_vBallVector.y) ;}
        float GetBallRotation() const {return m_vBallVector.z ;}

        void SetBallVector(sf::Vector3f _vector){m_vBallVector = _vector ;}

        void StateMachine(bool);
        void UpdateBallPosition(float);


    private:

        eBallState m_eCurrentBallState = eBallState::ResetGamePosition;
        eBallState m_eDesiredBallState = eBallState::None;
        sf::Vector3f m_vBallVector;
        sf::RectangleShape m_sShape;
        float m_fSpeed = 120.0f;
        float m_fTopSpeed = 20.00f;
        float m_fSpeedUpIncriment = 0.5f;

};