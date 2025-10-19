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

            pBat1->DetermDesiredMoveDirection( event, rTuple.pRenderWindow );
            pBat2->DetermDesiredMoveDirection( event, rTuple.pRenderWindow );
        }

        float fLapsedTime = rGameClock.getElapsedTime().asSeconds();
        pBat1->CalculateBatSpeed(rTuple.pRenderWindow, fLapsedTime);
        pBat2->CalculateBatSpeed(rTuple.pRenderWindow, fLapsedTime);

        
        rTuple.pRenderWindow->clear();
        rTuple.pRenderWindow->draw(GetCircleShape());
        if (pBat1->GetCurrentMoveDirection() == eBatMoveDirection::UP)
        {
            rTuple.pMessage->setString("UP");
        }
        else if (pBat1->GetCurrentMoveDirection() == eBatMoveDirection::DOWN)
        {
            rTuple.pMessage->setString("DOWN");
        }
        else
        {
            rTuple.pMessage->setString("NONE");
        }
        rTuple.pRenderWindow->draw( *rTuple.pMessage );
        rTuple.pRenderWindow->draw( pBat1->GetShape());
        rTuple.pRenderWindow->draw( pBat2->GetShape());
        rTuple.pRenderWindow->display();

        m_fTimeElapsed += rGameClock.getElapsedTime().asSeconds();

        rGameClock.restart();
    }

    return 1;
}

