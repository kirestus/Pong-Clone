#pragma once
#include <SFML/Graphics.hpp>
#include <headers/eBallstate.h>

class Ball
{
    public:

        Ball(std::shared_ptr<sf::RenderWindow> pRenderWindow);

        void SetDesiredBallState(eBallState _desiredState){ m_eDesiredBallState = _desiredState ;}

        sf::RectangleShape& ReferenceShape(){return m_sShape ;}

        sf::RectangleShape GetShape() const { return m_sShape; }
        void SetShape( sf::RectangleShape _shape ) { m_sShape = _shape ;}

        sf::RectangleShape* GetTrailShapeArray(){ return m_sShapeTrail ;}
        int GetTrailShapeArrayLength()const{ return m_iShapeTrailLen; }

        float GetXSpeed() const { return m_v2CurrentBallSpeed.x ;}
        void SetXSpeed( float _speed) { m_v2CurrentBallSpeed.x = _speed ;}

        float GetYSpeed() const { return m_v2CurrentBallSpeed.y ;}
        void SetYSpeed( float _speed) { m_v2CurrentBallSpeed.y = _speed ;}

        float GetTopSpeed() const { return m_fTopSpeed; }

        sf::Vector2f GetInitialSpeed() const { return m_v2InitialSpeed ;}

        sf::Vector2f GetTranslationPosition() const {return sf::Vector2f(m_vBallVector.x, m_vBallVector.y) ;}
        float GetBallRotation() const {return m_vBallVector.z ;}

        void SetBallVector(sf::Vector3f _vector){m_vBallVector = _vector ;}

        eBallState GetCurrentBallState() const { return m_eCurrentBallState ;}

        void StateMachine(const float);
        void UpdateBallPosition(const float, const bool);

        void OnBatCollision( const float );
        void OnWallCollision( const bool, const float );
        void OnScoreGoal(const bool, const bool isLeft, const float);

        void UpdateBallTrail(int);


    private:
        static const  int m_iShapeTrailLen = 6;
        eBallState m_eCurrentBallState = eBallState::ResetGamePosition;
        eBallState m_eDesiredBallState = eBallState::None;
        sf::Vector3f m_vBallVector;

        sf::RectangleShape m_sShape;
        sf::RectangleShape m_sShapeTrail[m_iShapeTrailLen+1];
        //+1 is so i can use the single length value for both creating the array and accesing the elements

        const float m_fTopSpeed = 15500.00f;
        const sf::Vector2f m_v2InitialSpeed = sf::Vector2f(4000.0f,00.0f);
        sf::Vector2f m_v2CurrentBallSpeed = m_v2InitialSpeed;
        const float m_fSpeedUpIncriment = 450.0f;

};