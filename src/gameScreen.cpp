#include <headers/gameScreen.h>

GameScreen::GameScreen(DataStruct& rTuple)
{
    CreateGameScreen(rTuple);
    UpdateGamescreen(rTuple,m_hClock);
}

//-----------------------------------------------------------------

void GameScreen::CreateGameScreen(DataStruct& rTuple)
{
    SetScreenCenter(CalculateScreenCenter(rTuple.pRenderWindow));
    rTuple.pWorldGameState = new GameState;
}

//-----------------------------------------------------------------

GameScreen::~GameScreen()
{
    delete m_pGameScreenGameState;
}

//-----------------------------------------------------------------

sf::Vector2f GameScreen::CalculateScreenCenter(sf::RenderWindow* pRenderWindow)
{
    sf::Vector2f vScreenCenter;
    vScreenCenter.x = pRenderWindow->getSize().x/2;
    vScreenCenter.y = pRenderWindow->getSize().y/2;

    return vScreenCenter;
}

//-----------------------------------------------------------------

int GameScreen::UpdateGamescreen(DataStruct& rTuple, sf::Clock &rGameClock)
{
    sf::Music wololo;
    wololo.openFromFile("sfx/song.mp3");
    wololo.play();

    Bat* pBat1 = rTuple.pBat1;
    Bat* pBat2 = rTuple.pBat2;
    Ball* pBall = rTuple.pBall;

    while (rTuple.pRenderWindow->isOpen())
    {
        sf::Event event;
        while (rTuple.pRenderWindow->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                rTuple.pRenderWindow->close();
            }
            Command* command = m_hInputHandler.HandleInput(&event, pBat1);
            if (command) 
            {  
                command->execute(rTuple);
            }
        }

        rTuple.pWorldGameState->DetermineGameState();
        float fLapsedTime = rGameClock.getElapsedTime().asSeconds();

        

        sf::RectangleShape pBallShape = pBall->ReferenceShape();

        if ( rTuple.pWorldGameState->GetCurrentGameState() != Paused )
        {
            pBat1->CalculateBatSpeed(rTuple.pRenderWindow, fLapsedTime);
            pBat2->CalculateBatSpeed(rTuple.pRenderWindow, fLapsedTime);
            pBall->UpdateBallPosition(fLapsedTime);

            bool bIsCollidingWithP1 = isBallCollidingWithTarget(rTuple.pBall->GetShape().getGlobalBounds(), rTuple.pBat1->GetShape().getGlobalBounds());
            bool bIsCollidingWithP2 = isBallCollidingWithTarget(rTuple.pBall->GetShape().getGlobalBounds(), rTuple.pBat2->GetShape().getGlobalBounds());

            if (bIsCollidingWithP1 || bIsCollidingWithP2)
            {
                sf::FloatRect testRect = rTuple.pBall->GetShape().getGlobalBounds();
                sf::FloatRect testRect2 = rTuple.pBat2->GetShape().getGlobalBounds();
                pBall->StateMachine(bIsCollidingWithP1 || bIsCollidingWithP2);
            }
        }
        
        rTuple.pMessage->setString(DebugTextGameState(rTuple.pWorldGameState->GetCurrentGameState()));
        rTuple.pRenderWindow->clear();

        rTuple.pRenderWindow->draw( *rTuple.pMessage );
        rTuple.pRenderWindow->draw( pBat1->GetShape());
        rTuple.pRenderWindow->draw( pBat2->GetShape());
        rTuple.pRenderWindow->draw( pBallShape );
        rTuple.pRenderWindow->display();

        m_fTimeElapsed += rGameClock.getElapsedTime().asSeconds();

        rGameClock.restart();
    }

    return 1;
}

//----------------------------------------------------------

sf::String GameScreen::DebugTextGameState( eGameState eGameState )
{
    if (eGameState == eGameState::Paused)
    {
        return sf::String("Paused:");
    }
    else if (eGameState == eGameState::Boot)
    {
        return sf::String("Boot:");
    }
    else if (eGameState == eGameState::Running)
    {
        return sf::String("Running:");
    }
    else if (eGameState == eGameState::GameOver)
    {
        return sf::String("GameOver:");
    }
    else
    {
        return sf::String("");
    }

    return sf::String("");
}

//----------------------------------------------------------

bool GameScreen::isBallCollidingWithTarget(sf::FloatRect box1, sf::FloatRect box2)
{
    if( box1.intersects(box2) )
    {
        return true;
    }
    else
    {
        return false;
    }
}