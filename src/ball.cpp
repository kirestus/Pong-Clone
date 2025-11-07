#include <headers/ball.h>
#include <assert.h>

Ball::Ball(std::shared_ptr<sf::RenderWindow> pRenderWindow)
{
    m_vBallVector = sf::Vector3f( pRenderWindow->getSize().x/2, pRenderWindow->getSize().y/2, 0 );
    m_sShape.setSize(sf::Vector2f(15.0f,15.0f));
    m_sShape.setFillColor(sf::Color::White);
    m_sShape.setOrigin(sf::Vector2f(m_sShape.getSize().x/2,m_sShape.getSize().y/2));
    m_sShape.setPosition(m_vBallVector.x ,m_vBallVector.y);
}

//----------------------------------------------------------

void Ball::StateMachine(float fScreenWidth)
{
    eBallState eNewBallState = m_eCurrentBallState;
    if (m_eCurrentBallState == eBallState::ResetGamePosition )
    {
        m_sShape.setPosition(m_vBallVector.x ,m_vBallVector.y);
        //game reset state will go to either left or right
        if (m_eDesiredBallState == eBallState::AtPlayer1)
        {
            eNewBallState = eBallState::AtPlayer1;
        }
        else if (m_eDesiredBallState == eBallState::AtPlayer2)
        {
            eNewBallState = eBallState::AtPlayer2;
        }
        else
        {
            eNewBallState = eBallState::LEFT;
        }
    }
    else if (m_eCurrentBallState == GoalOnPlayer1 )
    {
        eNewBallState = eBallState::AtPlayer1;
    }
        else if (m_eCurrentBallState == GoalOnPlayer2 )
    {
        eNewBallState = eBallState::AtPlayer2;
    }
    else if (m_eCurrentBallState == eBallState::AtPlayer1 )
    {
        if( m_eDesiredBallState == RIGHT) 
        {
            m_v2CurrentBallSpeed.x = abs(m_v2InitialSpeed.x)*-1;
            eNewBallState = m_eDesiredBallState;
        }
    }
    else if (m_eCurrentBallState == eBallState::AtPlayer2)
    {
        if (m_eDesiredBallState == LEFT)
        {
            m_v2CurrentBallSpeed.x = abs(m_v2InitialSpeed.x);
            eNewBallState = m_eDesiredBallState;
        }
    }
    else if(m_eCurrentBallState == LEFT || m_eCurrentBallState == RIGHT )
    {
        //check if our collision valid
        bool bIsCollisionValid = m_eCurrentBallState == LEFT && fScreenWidth/2 > GetTranslationPosition().x ||
        m_eCurrentBallState == RIGHT && fScreenWidth/2 <  m_vBallVector.x ;

        if ( m_eDesiredBallState == eBallState::HitBall && bIsCollisionValid )
        {
            //inverse the desired move state
            eNewBallState = m_eCurrentBallState == LEFT ? RIGHT : LEFT;
            m_v2CurrentBallSpeed.x *= -1;
            if (abs(m_v2CurrentBallSpeed.x) < m_fTopSpeed )
            {
                m_v2CurrentBallSpeed.x = m_v2CurrentBallSpeed.x > 0 ? m_v2CurrentBallSpeed.x += m_fSpeedUpIncriment : m_v2CurrentBallSpeed.x -= m_fSpeedUpIncriment;
            }
        }
        else if(m_eDesiredBallState == eBallState::HitWall)
        {
            SetYSpeed(GetYSpeed()*-1);
            eNewBallState = m_eCurrentBallState;
        }
        else if( m_eDesiredBallState == eBallState::GoalOnPlayer1)
        {
            eNewBallState = eBallState::AtPlayer2;
        }
        else if ( m_eDesiredBallState == eBallState::GoalOnPlayer2 )
        {
            eNewBallState = eBallState::AtPlayer1;
        }
    }

    assert(eNewBallState != eBallState::None && "StateMachine is NONE Update When Called to after scoring a goal");
    m_eCurrentBallState = eNewBallState;
}

//----------------------------------------------------------

void Ball::UpdateBallPosition(float fDeltaT, bool isPaused)
{
    if (isPaused)
    { 
        return; 
    }
    m_vBallVector.x -= GetXSpeed()*fDeltaT;
    m_vBallVector.y -= GetYSpeed()*fDeltaT;
    m_sShape.setPosition(GetTranslationPosition().x, GetTranslationPosition().y);
}

//----------------------------------------------------------

void Ball::OnBatCollision(float fScreenWidth)
{
    SetDesiredBallState(eBallState::HitBall);
    StateMachine(fScreenWidth);
}

//----------------------------------------------------------

void Ball::OnWallCollision(bool isColliding, float fScreenWidth)
{
    if (isColliding)
    {
        SetDesiredBallState(eBallState::HitWall);
        StateMachine(fScreenWidth);
    }
}

//----------------------------------------------------------

void Ball::OnScoreGoal(bool isColliding, bool isLeft, float fScreenWidth)
{
        if (isLeft)
        {
            SetDesiredBallState(eBallState::GoalOnPlayer1);
        }
        else
        {
            SetDesiredBallState(eBallState::GoalOnPlayer2);
        }
        StateMachine(fScreenWidth);
}