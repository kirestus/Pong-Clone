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
            Command* command = m_hInputHandler.HandleInput( &event );
            if (command) 
            {  
                command->execute(rTuple);
            }
        }

        rTuple.pWorldGameState->DetermineGameState();
        float fLapsedTime = rGameClock.getElapsedTime().asSeconds();

        sf::RectangleShape pBallShape = pBall->ReferenceShape();
        if (rTuple.pWorldGameState->GetCurrentGameState() == Boot)
        {
            ResetGame(rTuple);
        }

        if ( rTuple.pWorldGameState->GetCurrentGameState() != Paused && rTuple.pWorldGameState->GetCurrentGameState() != Boot )
        {
            pBat1->CalculateBatSpeed(rTuple.pRenderWindow, fLapsedTime);
            pBat2->CalculateBatSpeed(rTuple.pRenderWindow, fLapsedTime);
            pBall->UpdateBallPosition(fLapsedTime);
            CheckCollisions(rTuple);
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
    return false;
}

//----------------------------------------------------------

bool GameScreen::isBallHittingWall(sf::FloatRect box1, sf::RenderWindow* pRenderWindow)
{
    float rectTop = 0.0f;
    float rectBottom = pRenderWindow->getSize().y;
    if (box1.top+(box1.height) > rectBottom || box1.top < rectTop)
    {
        return true;
    }
    return false;
}

//----------------------------------------------------------

bool GameScreen::isBallHittingGoal( sf::FloatRect box1, sf::RenderWindow* pRenderWindow )
{
    float fLeftGoalPosition = 0.0f;
    float fRightGoalPosition = pRenderWindow->getSize().x;

    if  (box1.left < fLeftGoalPosition )
    {
        return true;
    }
    else if (box1.left+box1.width > fRightGoalPosition)
    {
        return true;
    }
    return false;
}

//----------------------------------------------------------

void GameScreen::ResetGame(DataStruct rTuple)
{
    sf::Vector2u vScreenArea = rTuple.pRenderWindow->getSize();
    rTuple.pBat1->SetPosition(sf::Vector2f(50.0f,vScreenArea.y/2));
    rTuple.pBat1->SetDesiredMoveDirection(eBatMoveDirection::NONE);

    rTuple.pBat2->SetDesiredMoveDirection(eBatMoveDirection::NONE);
    rTuple.pBat2->SetPosition(sf::Vector2f(vScreenArea.x-50.0f,vScreenArea.y/2) );

    rTuple.pBall->SetBallVector(sf::Vector3f(vScreenArea.x/2,vScreenArea.y/2, 0.00f));
    rTuple.pBall->SetXSpeed(rTuple.pBall->GetInitialSpeed().x);
    rTuple.pBall->SetYSpeed(rTuple.pBall->GetInitialSpeed().y);
}


void GameScreen::CheckCollisions(DataStruct rTuple)
{
    bool bIsCollidingWithP1 = isBallCollidingWithTarget(rTuple.pBall->GetShape().getGlobalBounds(), rTuple.pBat1->GetShape().getGlobalBounds());
    bool bIsCollidingWithP2 = isBallCollidingWithTarget(rTuple.pBall->GetShape().getGlobalBounds(), rTuple.pBat2->GetShape().getGlobalBounds());
    bool bIsCollidingWithWalls = isBallHittingWall(rTuple.pBall->GetShape().getGlobalBounds(), rTuple.pRenderWindow );
    bool bDidPLayerScore = isBallHittingGoal(rTuple.pBall->GetShape().getGlobalBounds(), rTuple.pRenderWindow );

    rTuple.pBall->OnBatCollision(bIsCollidingWithP1 || bIsCollidingWithP2);
    rTuple.pBall->OnWallCollision(bIsCollidingWithWalls);
    

    sf::Vector3f ballPosition(rTuple.pBall->GetTranslationPosition().x,rTuple.pBall->GetTranslationPosition().y,100.0f);
    rTuple.pPlayer1SoundEffect->setPosition(ballPosition);
    rTuple.pPlayer2SoundEffect->setPosition(ballPosition);

    if(bDidPLayerScore)
    {
        rTuple.pBall->OnScoreGoal(bDidPLayerScore, 0);
        rTuple.pWorldGameState->SetDesiredGamestate(eGameState::GameOver);
    }
    else if(bIsCollidingWithP1)
    {
        rTuple.pPlayer1SoundEffect->play();
    }
    else if(bIsCollidingWithP2)
    {

        rTuple.pPlayer2SoundEffect->play();
    }

}