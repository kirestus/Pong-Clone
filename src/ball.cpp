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
        //game reset state will go to either left or right
        if (m_eDesiredBallState == eBallState::LEFT)
        {
            eNewBallState = m_eDesiredBallState;
        }
        else if(m_eDesiredBallState == eBallState::RIGHT)
        {
            eNewBallState = m_eDesiredBallState;
        }
    }
    else if(m_eDesiredBallState == eBallState::HitWall)
    {
            SetYSpeed(GetYSpeed()*-1);
    }

    else if(m_eCurrentBallState == eBallState::LEFT || m_eCurrentBallState == eBallState::RIGHT)
    {
        if ( m_eDesiredBallState == eBallState::HitBall )
        {
            eBallState eOppositeDirection = m_eCurrentBallState == LEFT ? RIGHT : LEFT;
            // send the ball back the other way
            eNewBallState = eOppositeDirection;
            SetXSpeed((GetXSpeed()+25.0f)*-1);
            //todo change the balls rotation so the ball isnt just moving back and fourth
            //maybe add some degree of randomization to it or even calculate spin based on the paddle speed on contact
        }
        else if( m_eCurrentBallState == eBallState::GoalOnPlayer1 )
        {
            //todo Scoreboard handling will be done here
            eNewBallState = eBallState::AtPlayer1 ;
        }
        else if( m_eCurrentBallState == eBallState::GoalOnPlayer2 )
        {
            //todo Scoreboard handling will be done here
            eNewBallState = eBallState::AtPlayer2 ;
        }
    }
    m_eCurrentBallState = eNewBallState;
}

//----------------------------------------------------------

void Ball::UpdateBallPosition(float fDeltaT)
{
    // need to copy the physics over from the bat to stop the ball from being so
    m_vBallVector.x += GetXSpeed()*fDeltaT;
    m_vBallVector.y += GetYSpeed()*fDeltaT;
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

void Ball::OnWallCollision(bool isColliding)
{
    //i should really combine these 2 functions and take a param to check what i hit
    if (isColliding)
    {
        SetDesiredBallState(eBallState::HitWall);
        StateMachine();
    }
}

void Ball::OnScoreGoal(bool isColliding, int playerID)
{
    //i should really combine these 2 functions and take a param to check what i hit
    if (isColliding)
    {
        if (playerID == 1)
        {
            SetDesiredBallState(eBallState::GoalOnPlayer1);
        }
        else
        {
            SetDesiredBallState(eBallState::GoalOnPlayer2);
        }
        StateMachine();
    }
}