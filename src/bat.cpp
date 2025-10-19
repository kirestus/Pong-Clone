#include <headers/bat.h>

Bat::Bat(sf::Vector2f vPosition , ePlayerNumber ePlayer)
{
    m_hRectShape.setPosition(vPosition);
    m_vPosition = vPosition;
    m_hRectShape.setSize(sf::Vector2f(15.0f,80.0f));
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

void Bat::DetermDesiredMoveDirection( sf::Event event, sf::RenderWindow* pRenderWindow )
{

    if(event.type == sf::Event::KeyPressed )
    {
        if ( (m_ePlayerNumber == ePlayerNumber::PLAYER1 && (event.key.code ==  sf::Keyboard::W) 
        || m_ePlayerNumber == ePlayerNumber::PLAYER2 && (event.key.code ==  sf::Keyboard::Up)))
        {
           SetDesiredMoveDirection(eBatMoveDirection::UP); 
        }
        if ( (m_ePlayerNumber == ePlayerNumber::PLAYER1 && (event.key.code ==  sf::Keyboard::S)
        || m_ePlayerNumber == ePlayerNumber::PLAYER2 && (event.key.code ==  sf::Keyboard::Down)))
        {
           SetDesiredMoveDirection(eBatMoveDirection::DOWN); 
        }
    }
    
    if (event.type == event.KeyReleased)
    {
        if (m_ePlayerNumber == ePlayerNumber::PLAYER1 && (event.key.code ==  sf::Keyboard::W) ||
            (m_ePlayerNumber == ePlayerNumber::PLAYER2 && (event.key.code ==  sf::Keyboard::Up )))
        {
            if (m_eCurrentMoveDirection == eBatMoveDirection::UP)
            SetDesiredMoveDirection(eBatMoveDirection::NONE);
        }
        else if (m_ePlayerNumber == ePlayerNumber::PLAYER1 && (event.key.code ==  sf::Keyboard::S) ||
            (m_ePlayerNumber == ePlayerNumber::PLAYER2 && (event.key.code ==  sf::Keyboard::Down )))
        {
            if (m_eCurrentMoveDirection == eBatMoveDirection::DOWN)
            SetDesiredMoveDirection(eBatMoveDirection::NONE);
        }
    }  
}

//-----------------------------------------------------------------

eBatMoveDirection Bat::DetermCurrentMoveDirection(sf::RenderWindow* pRenderWindow)
{
    eBatMoveDirection eCurrentMoveDirection = GetCurrentMoveDirection();
    eBatMoveDirection eDesiredMoveDirection = GetDesiredMoveDirection();
    eBatMoveDirection eNewMoveDirection;

    switch(eCurrentMoveDirection)
    {
        case eBatMoveDirection::NONE :
            eNewMoveDirection = eDesiredMoveDirection;
            DecayVelocity();
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
    return (GetDesiredMoveDirection() == eBatMoveDirection::UP && fSpeed > 0 )||
    (GetDesiredMoveDirection() == eBatMoveDirection::DOWN && fSpeed < 0);
}

//-----------------------------------------------------------------

void Bat::CalculateBatSpeed(sf::RenderWindow* pRenderWindow, float fLapsedTime)
{
    eBatMoveDirection newMoveDirection = DetermCurrentMoveDirection(pRenderWindow);
    float fSpeed =  GetVelocity() * fLapsedTime;
    UpdateDesiredToShapeTransform();

    if (abs(fSpeed) < GetTopSpeed())
    {
        SetPosition(sf::Vector2f(GetPosition().x, GetPosition().y + fSpeed ));
    }

    if (newMoveDirection == eBatMoveDirection::UP )
    {
        ModifyVelocity( - abs( GetAccel() ));
    }
    else if (newMoveDirection == eBatMoveDirection::DOWN )
    { 
        ModifyVelocity( + abs( GetAccel() ));
    }

    if (IsBouncing(fSpeed))
    {
        DecayVelocity();
    }

    if( ( IsHittingBottom(pRenderWindow->getSize()) || IsHittingTop() ))
    {
        SetVelocity (GetVelocity()*-1.0 );
    }
    
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
    UpdateShapeToDesiredTransform();
}

//-----------------------------------------------------------------