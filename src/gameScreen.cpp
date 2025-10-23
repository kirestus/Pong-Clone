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

        if ( rTuple.pWorldGameState->GetCurrentGameState() != Paused )
        {
            pBat1->CalculateBatSpeed(rTuple.pRenderWindow, fLapsedTime);
            pBat2->CalculateBatSpeed(rTuple.pRenderWindow, fLapsedTime);
        }

        sf::RectangleShape pBallShape = pBall->ReferenceShape();
        
        rTuple.pRenderWindow->clear();
        if (rTuple.pWorldGameState->GetCurrentGameState() == eGameState::Paused)
        {
            rTuple.pMessage->setString("GAME PAUSED PRESS SPACE TO RESUME");
        }
        else if ((rTuple.pWorldGameState->GetCurrentGameState() == eGameState::Boot))
        {
            rTuple.pMessage->setString("BOOT");
        }
        else
        {
            rTuple.pMessage->setString("");
        }

            rTuple.pRenderWindow->draw( *rTuple.pMessage );
            rTuple.pRenderWindow->draw( pBat1->GetShape());
            rTuple.pRenderWindow->draw( pBat2->GetShape());
            rTuple.pRenderWindow->draw( pBall->GetShape() );
            rTuple.pRenderWindow->display();


        m_fTimeElapsed += rGameClock.getElapsedTime().asSeconds();

        rGameClock.restart();
    }

    return 1;
}

//----------------------------------------------------------