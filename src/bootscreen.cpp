#include <headers/bootscreen.h>

BootScreen::BootScreen(sf::RenderWindow *pRenderWindow){

    SetRenderWindow(pRenderWindow);
    CreateBootScreen(this);
    UpdateBootscreen(this,m_hClock);

};

sf::CircleShape BootScreen::CreateCircleShape(BootScreen* pBootScreen,float fradius)
{
    sf::CircleShape circle(30.0f);
    float circleMidPoint = circle.getRadius();
    circle.setOrigin(circleMidPoint,circleMidPoint);
    circle.setPosition(pBootScreen->GetScreenCenter());

    return circle;
}

void BootScreen::CreateBootScreen(BootScreen *pBootScreen)
{
    sf::RenderWindow *pRenderWindow = pBootScreen->GetRenderWindow();
    SetScreenCenter(pBootScreen->CalculateScreenCenter(pRenderWindow));

    sf::CircleShape circle = pBootScreen->CreateCircleShape(pBootScreen, 300.0);
    SetCircleShape(circle);

    CreateTextBox(pBootScreen);
}

void BootScreen::CreateTextBox(BootScreen *pBootScreen)
{
    sf::Font vector;
    vector.loadFromFile("/fonts/Vector.ttf");
    sf::Text text("Hello SFML", vector, 50);

}

sf::Vector2f BootScreen::CalculateScreenCenter(sf::RenderWindow* pRenderWindow)
{
    sf::Vector2f vScreenCenter;
    vScreenCenter.x = pRenderWindow->getSize().x/2;
    vScreenCenter.y = pRenderWindow->getSize().y/2;

    return vScreenCenter;
}

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
                    m_eDesiredMoveDirection = MoveDirection::UP;
                break;
                
                case sf::Keyboard::S:
                    m_eDesiredMoveDirection = MoveDirection::DOWN;
                break;
                
                default:

                break;
            }
        }
        else if (event.type == event.KeyReleased)
        {
            m_eDesiredMoveDirection = MoveDirection::NONE;
        }
    }
}

int BootScreen::UpdateBootscreen(BootScreen *pBootScreen, sf::Clock &rGameClock)
{

    sf::RenderWindow *pRenderWindow = GetRenderWindow();

    sf::Font vector;
    vector.loadFromFile("fonts/Vector.ttf");
    static sf::Text tMessage = pBootScreen->GetMessageText();
    tMessage.setFont(GetMessageFont());
    tMessage.setString("Pong");
    tMessage.setFillColor(sf::Color::White);
    tMessage.setCharacterSize(52);
    tMessage.setFont(vector);

    while (pRenderWindow->isOpen())
    {
        DetermineEvent(pRenderWindow);
        m_vPosition = tMessage.getPosition();
        if (m_eDesiredMoveDirection == MoveDirection::UP)
        {
            m_fVelocity -= abs(m_fYaccel);
        }
        else if (m_eDesiredMoveDirection == MoveDirection::DOWN )
        {
            m_fVelocity += abs(m_fYaccel);
        }
        else
        {
             m_fVelocity *= m_fYdecaySpeed;
        }

        float fSpeed = m_fVelocity * rGameClock.getElapsedTime().asSeconds();
        
        m_vPosition.y += fSpeed;
        tMessage.setPosition(tMessage.getPosition().x, m_vPosition.y);
        pRenderWindow->clear();
        pRenderWindow->draw(GetCircleShape());
        pRenderWindow->draw(tMessage);
        pRenderWindow->display();

        m_fTimeElapsed += rGameClock.getElapsedTime().asSeconds();

        rGameClock.restart();
    }

    return 1;
}

