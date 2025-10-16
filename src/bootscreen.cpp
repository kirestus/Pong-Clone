#include <headers/bootscreen.h>
#include <headers/bat.h>
#include <headers/tuple.h>

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

void BootScreen::DetermineEvent(sf::RenderWindow *pRenderWindow)
{
    sf::Event event;
    while (pRenderWindow->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            pRenderWindow->close();

        if(event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
                case sf::Keyboard::W:
                    m_eDesiredMoveDirection = eBatMoveDirection::UP;
                break;
                
                case sf::Keyboard::S:
                    m_eDesiredMoveDirection = eBatMoveDirection::DOWN;
                break;
                
                default:

                break;
            }
        }
        else if (event.type == event.KeyReleased && 
            event.key.code ==  sf::Keyboard::W || event.key.code == sf::Keyboard::S)
        {
                m_eDesiredMoveDirection = eBatMoveDirection::NONE;
        }
    }
}
//-----------------------------------------------------------------

int BootScreen::UpdateBootscreen(tuple rTuple, sf::Clock &rGameClock)
{
    sf::Music wololo;
    wololo.openFromFile("sfx/song.mp3");
    wololo.play();

    while (rTuple.pRenderWindow->isOpen())
    {
        DetermineEvent(rTuple.pRenderWindow);
        m_vPosition =  rTuple.pMessage->getPosition();
        if (m_eDesiredMoveDirection == eBatMoveDirection::UP)
        {
            m_fVelocity -= abs(m_fYaccel);
        }
        else if (m_eDesiredMoveDirection == eBatMoveDirection::DOWN )
        {
            m_fVelocity += abs(m_fYaccel);
        }
        else
        {
             m_fVelocity *= m_fYdecaySpeed;
        }

        float fSpeed = m_fVelocity * rGameClock.getElapsedTime().asSeconds();
        
        m_vPosition.y += fSpeed;
        rTuple.pMessage->setPosition( rTuple.pMessage->getPosition().x, m_vPosition.y);
        rTuple.pRenderWindow->clear();
        rTuple.pRenderWindow->draw(GetCircleShape());
        rTuple.pRenderWindow->draw( *rTuple.pMessage );
        rTuple.pRenderWindow->display();

        m_fTimeElapsed += rGameClock.getElapsedTime().asSeconds();

        rGameClock.restart();
    }

    return 1;
}

