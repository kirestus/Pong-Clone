#include <headers/bat.h>


Bat::Bat(sf::Vector2f vPosition , ePlayerNumber ePlayer)
{
    m_hRectShape.setPosition(vPosition);
    m_vPosition = vPosition;
    m_hRectShape.setSize(sf::Vector2f(15.0f,150.0f));
    m_hRectShape.setFillColor(sf::Color::White);
    m_hRectShape.setOrigin( sf::Vector2f(m_hRectShape.getSize().x/2, m_hRectShape.getSize().y/2));
    m_ePlayerNumber = ePlayer;
}

//-----------------------------------------------------------------

Bat::Bat()
{
    m_vPosition = sf::Vector2f(70,300);
    m_hRectShape.setSize(sf::Vector2f(15.0f,80.0f));
    m_hRectShape.setFillColor(sf::Color::White);
    m_hRectShape.setOrigin( sf::Vector2f(m_hRectShape.getSize().x/2, m_hRectShape.getSize().y/2));
    m_ePlayerNumber = ePlayerNumber::PLAYER1; // if undefined assume player 1
}

//-----------------------------------------------------------------

eBatMoveDirection Bat::DetermCurrentMoveDirection(std::shared_ptr<sf::RenderWindow> pRenderWindow)
{
    eBatMoveDirection eCurrentMoveDirection = GetCurrentMoveDirection();
    eBatMoveDirection eDesiredMoveDirection = GetDesiredMoveDirection();
    eBatMoveDirection eNewMoveDirection;

    switch(eCurrentMoveDirection)
    {
        case eBatMoveDirection::NONE :
            eNewMoveDirection = eDesiredMoveDirection;
            break;
        case eBatMoveDirection::DOWN :
        {
            if (IsHittingBottom(pRenderWindow->getSize()))
            {
                eNewMoveDirection = eBatMoveDirection::UP;
            }
            else
            {
                eNewMoveDirection = eDesiredMoveDirection;
            }
            break;
        }
        case eBatMoveDirection::UP :
        {
            if ( IsHittingTop() )
            {
                eNewMoveDirection = eBatMoveDirection::NONE;
            }
            else
            {
                eNewMoveDirection = eDesiredMoveDirection;
            }
            break;
        }
        default:
            sf::err;
            break;   
    }
    SetCurrentMoveDirection(eNewMoveDirection);
    return eNewMoveDirection;
}

//-----------------------------------------------------------------

bool Bat::IsBouncing(float fSpeed)
{
    return ((GetDesiredMoveDirection() == eBatMoveDirection::UP && fSpeed > 0 )||
    (GetDesiredMoveDirection() == eBatMoveDirection::DOWN && fSpeed < 0));
}

//-----------------------------------------------------------------

void Bat::CalculateBatSpeed(std::shared_ptr<sf::RenderWindow> pRenderWindow, float fLapsedTime, bool isGamePaused = false)
{
    eBatMoveDirection newMoveDirection = DetermCurrentMoveDirection(pRenderWindow);
    //UpdateDesiredToShapeTransform();
    float fSpeed =  GetVelocity()*fLapsedTime;
    if (!isGamePaused)
    {
        if (( fSpeed < GetTopSpeed() || fSpeed > GetTopSpeed() *-1 ))
        {
            SetPosition(sf::Vector2f(GetPosition().x, GetPosition().y + fSpeed ));
        }

        if (newMoveDirection == eBatMoveDirection::UP && abs(fSpeed) < GetTopSpeed())
        {
            ModifyVelocity( - ( GetAccel()*fLapsedTime ));
        }
        else if (newMoveDirection == eBatMoveDirection::DOWN && abs(fSpeed) < GetTopSpeed())
        { 
            ModifyVelocity(  ( GetAccel()*fLapsedTime ));
        }
        else
        {
            SetVelocity(fSpeed*0.96/fLapsedTime);
        }

        if( ( IsHittingBottom(pRenderWindow->getSize()) || IsHittingTop() ))
        {
            NudgeBat(pRenderWindow);
            SetVelocity(0.0f);
        }
        
        UpdateShapeToDesiredTransform();
    }

}

//-----------------------------------------------------------------


void Bat::NudgeBat(std::shared_ptr<sf::RenderWindow> pRenderWindow)
{
        //Nudge the bat if its at the top or bottom so it doesnt glitch out
    if( IsHittingTop() )
    {
        sf::Vector2f nudgeDown = sf::Vector2f(GetPosition().x,GetPosition().y+0.1);
        SetPosition(nudgeDown);
    }
    else if( IsHittingBottom( pRenderWindow->getSize() ) )
    {
        sf::Vector2f nudgeUp = sf::Vector2f(GetPosition().x,GetPosition().y-0.1);
        SetPosition(nudgeUp);
    }
}

//-----------------------------------------------------------------

void Bat::UpdateHitVFX(std::shared_ptr<sf::RenderWindow> pRenderWindow, int iSimFrame)
{

    static const int iFXFrameTime = 20;
    //todo change color to more of a red the closer to the edge of the paddle that the ball is hit
    //i will later tie this into ball controll so hits near the edge have more spread and the middle is the sweet spot

    for(int i = m_iHitFXArrayLength; i >= 0 ; i--)
    {
        if ( m_iLastFrameBallWasHit + iFXFrameTime > iSimFrame)
        {
            if(GetPlayerNumber() == PLAYER2)
            {
                m_FXShape[i].setScale(sf::Vector2f(-1.0f,1.0f));
            }
            else
            {
                m_FXShape[i].setScale(sf::Vector2f(1.0f,1.0f));
            }
            m_FXShape[i].setFillColor( sf::Color( 150,150,255,10 ) );
            m_FXShape[i].setPosition( m_hRectShape.getPosition().x ,m_hRectShape.getPosition().y );
            m_FXShape[i].setSize( sf::Vector2f( m_hRectShape.getSize().x*i*0.8 +10 , m_hRectShape.getSize().y-(i*8)));
            
            if(GetPlayerNumber() == PLAYER2)
            {
                m_FXShape[i].scale(sf::Vector2f(m_FXShape->getScale().x * -1, 1.0));
            }

            m_FXShape[i].setOrigin( sf::Vector2f(0, m_FXShape[i].getSize().y/2 ));
        }
        else
        {
            // todo: going to tie the scale back on the x to the balls speed on hit
            m_FXShape[i].scale(sf::Vector2f(0.85f,0.99f));
        }

    }
}