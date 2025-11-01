#include <headers/ball.h>

Ball::Ball(sf::RenderWindow* pRenderWindow)
{
    m_vBallVector = sf::Vector3f( pRenderWindow->getSize().x/2, pRenderWindow->getSize().y/2, 0 );
    m_sShape.setSize(sf::Vector2f(15.0f,15.0f));
    m_sShape.setFillColor(sf::Color::White);
    m_sShape.setOrigin(sf::Vector2f(m_sShape.getSize().x/2,m_sShape.getSize().y/2));
    m_sShape.setPosition(m_vBallVector.x ,m_vBallVector.y);
}

//----------------------------------------------------------

void Ball::StateMachine()
{
    eBallState eNewBallState;
    if (m_eCurrentBallState == eBallState::ResetGamePosition )
    {
            m_sShape.setPosition(m_vBallVector.x ,m_vBallVector.y);
        //game reset state will go to either left or right
        if (m_eDesiredBallState == eBallState::RIGHT)
        {
            eNewBallState = eBallState::RIGHT;
        }
        else
        {
            eNewBallState = eBallState::LEFT;
        }
    }
    else if(m_eCurrentBallState == eBallState::LEFT || m_eCurrentBallState == eBallState::RIGHT)
    {
        if ( m_eDesiredBallState == eBallState::HitBall )
        {
            eBallState eOppositeDirection = m_eCurrentBallState == LEFT ? RIGHT : LEFT;
            // send the ball back the other way


            if (eOppositeDirection == LEFT && (GetShape().getGlobalBounds().getPosition().x > 300.0f) ||
             (eOppositeDirection == RIGHT &&  (GetShape().getGlobalBounds().getPosition().x < 300.0f)))
            {
                eNewBallState = eOppositeDirection;

                if (abs(m_v2CurrentBallSpeed.x) < m_fTopSpeed )
                {
                    m_v2CurrentBallSpeed.x = eOppositeDirection == LEFT ? m_v2CurrentBallSpeed.x -= m_fSpeedUpIncriment : 
                    m_v2CurrentBallSpeed.x += m_fSpeedUpIncriment; 
                }

                m_v2CurrentBallSpeed.x *= -1;
                SetYSpeed(GetYSpeed()+20.0f);
            }
            else
            {
                eNewBallState = m_eCurrentBallState;
            }
           
            //todo change the balls rotation so the ball isnt just moving back and fourth
            //maybe add some degree of randomization to it or even calculate spin based on the paddle speed on contact
        }
        else if( m_eCurrentBallState == eBallState::GoalOnPlayer1 ||
        m_eCurrentBallState == eBallState::GoalOnPlayer2 )
        {
            eNewBallState = eBallState::ResetGamePosition;
        }
        else if(m_eDesiredBallState == eBallState::HitWall)
        {
            SetYSpeed(GetYSpeed()*-1);
            eNewBallState = m_eCurrentBallState;
        }
    }
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

void Ball::OnBatCollision(bool isColliding)
{
    if (isColliding)
    {
        SetDesiredBallState(eBallState::HitBall);
        StateMachine();
    }
}

//----------------------------------------------------------

void Ball::OnWallCollision(bool isColliding)
{
    if (isColliding)
    {
        SetDesiredBallState(eBallState::HitWall);
        StateMachine();
    }
}

//----------------------------------------------------------

void Ball::OnScoreGoal(bool isColliding, bool isLeft)
{
        if (isLeft)
        {
            SetDesiredBallState(eBallState::GoalOnPlayer1);
        }
        else
        {
            SetDesiredBallState(eBallState::GoalOnPlayer2);
        }
        StateMachine();
}