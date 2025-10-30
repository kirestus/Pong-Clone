#pragma once
#include <SFML/Graphics.hpp>

enum eBallState{
    None,
    LEFT,
    RIGHT,
    HitBall,
    HitWall,
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

        void SetDesiredBallState(eBallState _desiredState){ m_eDesiredBallState = _desiredState ;}

        sf::RectangleShape& ReferenceShape(){return m_sShape ;}

        sf::RectangleShape GetShape() const { return m_sShape; }
        void SetShape( sf::RectangleShape _shape ) { m_sShape = _shape ;}

        float GetXSpeed() const { return m_v2CurrentBallSpeed.x ;}
        void SetXSpeed( float _speed) { m_v2CurrentBallSpeed.x = _speed ;}

        float GetYSpeed() const { return m_v2CurrentBallSpeed.y ;}
        void SetYSpeed( float _speed) { m_v2CurrentBallSpeed.y = _speed ;}

        sf::Vector2f GetInitialSpeed() const { return m_v2InitialSpeed ;}


        sf::Vector2f GetTranslationPosition() const {return sf::Vector2f(m_vBallVector.x, m_vBallVector.y) ;}
        float GetBallRotation() const {return m_vBallVector.z ;}

        void SetBallVector(sf::Vector3f _vector){m_vBallVector = _vector ;}


        eBallState GetCurrentBallState() const { return m_eCurrentBallState ;}

        void StateMachine();
        void UpdateBallPosition(float);

        void OnBatCollision(bool);
        void OnWallCollision(bool);
        void OnScoreGoal(bool, int playerID);


    private:

        eBallState m_eCurrentBallState = eBallState::ResetGamePosition;
        eBallState m_eDesiredBallState = eBallState::None;
        sf::Vector3f m_vBallVector;
        sf::RectangleShape m_sShape;
        const float m_fTopSpeed = 8200.00f;
        const sf::Vector2f m_v2InitialSpeed = sf::Vector2f(3000.0f,600.0f);
        sf::Vector2f m_v2CurrentBallSpeed = m_v2InitialSpeed;
        const float m_fSpeedUpIncriment = 600.0f;

};