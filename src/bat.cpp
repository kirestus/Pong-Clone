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

bool Bat::IsBouncing()
{
    const float fSpeed = GetVelocity();
    return ((GetDesiredMoveDirection() == eBatMoveDirection::UP && fSpeed > 0 )||
    (GetDesiredMoveDirection() == eBatMoveDirection::DOWN && fSpeed < 0));
}

//-----------------------------------------------------------------

void Bat::UpdateHitVFX(const float fDeltaT, bool bHitSweetSpot )
{
    //pass tuple to this instead of all this other shit
    constexpr uint8 iFXFrameTime = 20;

    //todo change color to more of a red the closer to the edge of the paddle that the ball is hit
    //i will later tie this into ball controll so hits near the edge have more spread and the middle is the sweet spot

    for(int8 i = m_iHitFXArrayLength; i >= 0 ; i--)
    {
        if ( m_iLastFrameBallWasHit + iFXFrameTime > fDeltaT)
        {
            if(GetPlayerNumber() == ePlayerNumber::PLAYER2)
            {
                m_FXShape[i].setScale(sf::Vector2f(-1.0f,1.0f));
            }
            else
            {
                m_FXShape[i].setScale(sf::Vector2f(1.0f,1.0f));
            }
            
            // TODO: pass the data tuple instead and get the sweetspot hit
            sf::Color FillColor;
            if(GetPlayerNumber()== ePlayerNumber::PLAYER1)
            {
                FillColor = bHitSweetSpot ? sf::Color( 255,100,100,16 ) : sf::Color( 255,255,255,10 );
            }
            else
            {
                FillColor = bHitSweetSpot ? sf::Color( 100,100,255,16 ) : sf::Color( 255,255,255,10 );
            }
            const float fSizeTweak = bHitSweetSpot ? 0.66f : 0.5f;

            m_FXShape[i].setFillColor( FillColor );
            m_FXShape[i].setPosition( m_hRectShape.getPosition().x ,m_hRectShape.getPosition().y );
            m_FXShape[i].setSize( sf::Vector2f( m_hRectShape.getSize().x*i*fSizeTweak+10.0f , m_hRectShape.getSize().y-(i*fSizeTweak*10.0f)));
            
            if(GetPlayerNumber() == ePlayerNumber::PLAYER2)
            {
                m_FXShape[i].scale(sf::Vector2f(m_FXShape->getScale().x * -1.0f, 1.0f));
            }

            m_FXShape[i].setOrigin( sf::Vector2f(0, m_FXShape[i].getSize().y/2.0f ));
        }
        else
        {
            // todo: going to tie the scale back on the x to the balls speed on hit
            m_FXShape[i].scale(sf::Vector2f(0.85f,0.99f));
        }

    }
}

//-----------------------------------------------------------------
