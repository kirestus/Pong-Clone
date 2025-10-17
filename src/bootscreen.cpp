#include <headers/bootscreen.h>

BootScreen::BootScreen(tuple rTuple)
{
    CreateBootScreen(rTuple);
    UpdateBootscreen(rTuple,m_hClock);
};
//-----------------------------------------------------------------

sf::CircleShape BootScreen::CreateCircleShape(BootScreen* pBootScreen,float fradius)
{
    sf::CircleShape circle(30.0f);
    float circleMidPoint = circle.getRadius();
    circle.setOrigin(circleMidPoint,circleMidPoint);
    circle.setPosition(pBootScreen->GetScreenCenter());

    return circle;
}
//-----------------------------------------------------------------

void BootScreen::CreateBootScreen(tuple rTuple)
{
    SetScreenCenter(CalculateScreenCenter(rTuple.pRenderWindow));

    sf::CircleShape circle = CreateCircleShape(this, 300.0);
    SetCircleShape(circle);
}

//-----------------------------------------------------------------

sf::Vector2f BootScreen::CalculateScreenCenter(sf::RenderWindow* pRenderWindow)
{
    sf::Vector2f vScreenCenter;
    vScreenCenter.x = pRenderWindow->getSize().x/2;
    vScreenCenter.y = pRenderWindow->getSize().y/2;

    return vScreenCenter;
}
//-----------------------------------------------------------------

void BootScreen::DetermineEvent(sf::RenderWindow *pRenderWindow, Bat *pBat, ePlayerNumber playerId, sf::Event event)
{
    if(event.type == sf::Event::KeyPressed )
    {
        switch (event.key.code)
        {
            if ( playerId == ePlayerNumber::PLAYER1 )
            {
                case sf::Keyboard::W:
                    pBat->SetMovementDirection(eBatMoveDirection::UP);
                break;
            
                case sf::Keyboard::S:
                    pBat->SetMovementDirection(eBatMoveDirection::DOWN);
                break;
            }
            else if ( playerId == ePlayerNumber::PLAYER2 )
            {
                case sf::Keyboard::Up:
                    pBat->SetMovementDirection(eBatMoveDirection::UP);
                break;
            
                case sf::Keyboard::Down:
                    pBat->SetMovementDirection(eBatMoveDirection::DOWN);
                break;
            }
            default:

                break;
        }
    }
    
    if (event.type == event.KeyReleased)
    {
        if ((playerId == ePlayerNumber::PLAYER1 && event.key.code ==  sf::Keyboard::W || event.key.code == sf::Keyboard::S) ||
            (playerId == ePlayerNumber::PLAYER2 && event.key.code ==  sf::Keyboard::Up || event.key.code == sf::Keyboard::Down))
        {
            pBat->SetMovementDirection(eBatMoveDirection::NONE);
        }
    } 
    
}

//-----------------------------------------------------------------

static void CalculateBatSpeed(sf::RenderWindow *pRenderWindow, Bat *pBat, float fLapsedTime)
{
    pBat->UpdateShapeToDesiredTransform();

        if (pBat->GetMovementDirection() == eBatMoveDirection::UP )
        {
            pBat->ModifyVelocity( - abs( pBat->GetAccel() ));

        }
        else if (pBat->GetMovementDirection() == eBatMoveDirection::DOWN )
        {
            pBat->ModifyVelocity( abs( pBat->GetAccel() ));
        }
        else
        {
            pBat->DecayVelocity();
        }

        float fSpeed = pBat->GetVelocity() * fLapsedTime;
        
        pBat->SetPosition(sf::Vector2f(pBat->GetPosition().x, pBat->GetPosition().y + fSpeed));
        pBat->UpdateShapeToDesiredTransform();
}

//-----------------------------------------------------------------

int BootScreen::UpdateBootscreen(tuple rTuple, sf::Clock &rGameClock)
{
    sf::Music wololo;
    wololo.openFromFile("sfx/song.mp3");
    wololo.play();

    Bat* pBat1 = rTuple.pBat1;
    Bat* pBat2 = rTuple.pBat2;

    while (rTuple.pRenderWindow->isOpen())
    {
        sf::Event event;
        while (rTuple.pRenderWindow->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                rTuple.pRenderWindow->close();
            }

            DetermineEvent(rTuple.pRenderWindow, pBat1, ePlayerNumber::PLAYER1, event);
            DetermineEvent(rTuple.pRenderWindow, pBat2, ePlayerNumber::PLAYER2, event);
        }

        float fLapsedTime = rGameClock.getElapsedTime().asSeconds();
        CalculateBatSpeed(rTuple.pRenderWindow, pBat1, fLapsedTime);
        CalculateBatSpeed(rTuple.pRenderWindow, pBat2, fLapsedTime);

        
        rTuple.pRenderWindow->clear();
        rTuple.pRenderWindow->draw(GetCircleShape());
        rTuple.pRenderWindow->draw( *rTuple.pMessage );
        rTuple.pRenderWindow->draw( pBat1->GetShape());
        rTuple.pRenderWindow->draw( pBat2->GetShape());
        rTuple.pRenderWindow->display();

        m_fTimeElapsed += rGameClock.getElapsedTime().asSeconds();

        rGameClock.restart();
    }

    return 1;
}

