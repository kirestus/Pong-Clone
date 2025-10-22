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

void Ball::StateMachine(eBallState eCurrentBallState, eBallState eDesiredBallState)
{
    eBallState eNewBallState;
    if (eCurrentBallState == eBallState::ResetGamePosition )
    {
        //game reset state will go to either left or right
        if (eDesiredBallState == eBallState::LEFT)
        {
            eNewBallState = eDesiredBallState;
        }
        else if(eDesiredBallState == eBallState::RIGHT)
        {
            eNewBallState = eDesiredBallState;
        }
    }
    else if( eCurrentBallState == eBallState::LEFT || eCurrentBallState == eBallState::RIGHT )
    {
        if ( eDesiredBallState == eBallState::LEFT || eCurrentBallState == eBallState::RIGHT)
        {
            if(eNewBallState != eCurrentBallState)
            {
                // send the ball back the other way
                eNewBallState = eDesiredBallState ;
                m_fSpeed *= -1;
                //todo change the balls rotation so the ball isnt just moving back and fourth
                //maybe add some degree of randomization to it or even calculate spin based on the paddle speed on contact
            }
        }
        else if( eCurrentBallState == eBallState::GoalOnPlayer1 )
        {
            //todo Scoreboard handling will be done here
            eNewBallState = eBallState::AtPlayer1 ;
        }
        else if( eCurrentBallState == eBallState::GoalOnPlayer2 )
        {
            //todo Scoreboard handling will be done here
            eNewBallState = eBallState::AtPlayer2 ;
        }
    }
}

//----------------------------------------------------------

void Ball::UpdateBallPosition()
{
    sf::Vector2f vBallPosition = GetTranslationPosition();
    float fBallRotation = GetBallRotation();

    sf::RectangleShape sBallShape = GetShape();

}