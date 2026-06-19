#pragma once
#include <SFML/Graphics.hpp>
#include <headers/eBallstate.h>
#include <headers/collision.h>
#include <headers/alias.h>

class Ball
{
    public:

        Ball(std::shared_ptr<sf::RenderWindow> pRenderWindow);

        sf::RectangleShape& ReferenceShape(){return m_sShape ;}

        sf::RectangleShape GetShape() const { return m_sShape; }
        void SetShape( sf::RectangleShape _shape ) { m_sShape = _shape ;}

        sf::RectangleShape* GetTrailShapeArray(){ return m_sShapeTrail ;}
        uint8 GetTrailShapeArrayLength()const{ return m_iShapeTrailLen; }

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
        eBallState GetDesiredBallState() const { return m_eDesiredBallState ;}

        eCollisionType GetLastCollisionType() const { return m_eLastCollisionType ;}
        void SetLastCollisiontype(eCollisionType type){m_eLastCollisionType = type;}

        void SetCurrentBallState(eBallState state){ m_eCurrentBallState = state ;}
        void SetDesiredBallState(eBallState _desiredState){ m_eDesiredBallState = _desiredState ;}


        void StateMachine(const float);
        void UpdateBallPosition(const float, const bool);

        void OnBatCollision( const float );
        void OnWallCollision( const bool, const float );
        void OnScoreGoal(const bool, const bool isLeft, const float);

        bool IsLastHitOnSweetSpot() const { return m_bIsLastHitOnSweetSpot ;}
        void SetLastHitOnSweetSpot(bool isSweet){m_bIsLastHitOnSweetSpot = isSweet;}

        void UpdateBallTrail(int64);


    private:
        static constexpr uint8 m_iShapeTrailLen = 6;
        eBallState m_eCurrentBallState = eBallState::ResetGamePosition;
        eBallState m_eDesiredBallState = eBallState::None;
        eCollisionType m_eLastCollisionType = eCollisionType::NoCollision;

        sf::Vector3f m_vBallVector;

        sf::RectangleShape m_sShape;
        sf::RectangleShape m_sShapeTrail[m_iShapeTrailLen+1];
        //+1 is so i can use the single length value for both creating the array and accesing the elements

        static constexpr float m_fTopSpeed = 15500.00f;
        const sf::Vector2f m_v2InitialSpeed = sf::Vector2f(4000.0f,00.0f);
        sf::Vector2f m_v2CurrentBallSpeed = m_v2InitialSpeed;
        static constexpr float m_fSpeedUpIncriment = 600.0f;

        bool m_bIsLastHitOnSweetSpot = false;

};