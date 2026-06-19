#include <headers/bat.h>


Bat::Bat(const sf::Vector2f vPosition , const ePlayerNumber ePlayer)
{
    m_hRectShape.setPosition(vPosition);
    m_vPosition = vPosition;
    m_hRectShape.setSize(sf::Vector2f(15.0f,150.0f));
    m_hRectShape.setFillColor(sf::Color::White);
    m_hRectShape.setOrigin( sf::Vector2f(m_hRectShape.getSize().x/2, m_hRectShape.getSize().y/2));
    m_ePlayerNumber = ePlayer;
}

//-----------------------------------------------------------------

eBatMoveDirection Bat::DetermCurrentMoveDirection(const std::shared_ptr<sf::RenderWindow> pRenderWindow)
{
    const eBatMoveDirection eCurrentMoveDirection = GetCurrentMoveDirection();
    const eBatMoveDirection eDesiredMoveDirection = GetDesiredMoveDirection();
    eBatMoveDirection eNewMoveDirection;

    switch(eCurrentMoveDirection)
    {
        case eBatMoveDirection::NONE :
            eNewMoveDirection = eDesiredMoveDirection;
            break;
        case eBatMoveDirection::DOWN :
        {
            eNewMoveDirection = IsHittingBottom(pRenderWindow->getSize() ) ? eBatMoveDirection::UP : eNewMoveDirection = eDesiredMoveDirection;
            break;
        }
        case eBatMoveDirection::UP :
        {
            eNewMoveDirection = IsHittingTop() ? eBatMoveDirection::NONE : eNewMoveDirection = eDesiredMoveDirection;
            break;
        }
        default:
            break;   
    }
    SetCurrentMoveDirection(eNewMoveDirection); // should i be setting this here if its returning the value?
    return eNewMoveDirection;
}

//-----------------------------------------------------------------

bool Bat::IsBouncing(const float fSpeed)
{
    return ((GetDesiredMoveDirection() == eBatMoveDirection::UP && fSpeed > 0 )||
    (GetDesiredMoveDirection() == eBatMoveDirection::DOWN && fSpeed < 0));
}

//-----------------------------------------------------------------

void Bat::CalculateBatSpeed(const std::shared_ptr<sf::RenderWindow> pRenderWindow, const float fLapsedTime, const bool isGamePaused = false)
{
    eBatMoveDirection newMoveDirection = DetermCurrentMoveDirection(pRenderWindow);
    //UpdateDesiredToShapeTransform();
    const float fSpeed =  GetVelocity()*fLapsedTime;
    if (!isGamePaused)
    {
        if ( abs(fSpeed) < GetTopSpeed())
        {
            SetPosition(sf::Vector2f(GetPosition().x, GetPosition().y + fSpeed ));
        }

        if (newMoveDirection == eBatMoveDirection::UP && abs(fSpeed) < GetTopSpeed())
        {
            ModifyVelocity( - ( GetAccel()*fLapsedTime ));
        }
        else if (newMoveDirection == eBatMoveDirection::DOWN && abs(fSpeed) < GetTopSpeed())
        { 
            ModifyVelocity(( GetAccel()*fLapsedTime ));
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


void Bat::NudgeBat(const std::shared_ptr<sf::RenderWindow> pRenderWindow)
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

void Bat::UpdateHitVFX(const std::shared_ptr<sf::RenderWindow> pRenderWindow, long iSimFrame, float fLastHitYPosition)
{

    constexpr uint8 iFXFrameTime = 20;

    //todo change color to more of a red the closer to the edge of the paddle that the ball is hit
    //i will later tie this into ball controll so hits near the edge have more spread and the middle is the sweet spot

    for(int8 i = m_iHitFXArrayLength; i >= 0 ; i--)
    {
        if ( m_iLastFrameBallWasHit + iFXFrameTime > iSimFrame)
        {
            if(GetPlayerNumber() == ePlayerNumber::PLAYER2)
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
            
            if(GetPlayerNumber() == ePlayerNumber::PLAYER2)
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

//-----------------------------------------------------------------

