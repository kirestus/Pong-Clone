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

        bool bIsPaused = rTuple.pWorldGameState->GetCurrentGameState() == eGameState::Paused;
        
        pBall->UpdateBallPosition(fLapsedTime, bIsPaused);
        pBat1->CalculateBatSpeed(rTuple.pRenderWindow, fLapsedTime, bIsPaused);
        pBat2->CalculateBatSpeed(rTuple.pRenderWindow, fLapsedTime, bIsPaused);
        CheckCollisions(rTuple);
        
        //rTuple.pMessage->setString(DebugTextGameState(rTuple.pWorldGameState->GetCurrentGameState()));
        //rTuple.pMessage->setString(DebugTextBallState(rTuple.pBall->GetCurrentBallState()));
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

void GameScreen::ResetGame(DataStruct rTuple)
{
    sf::Vector2u vScreenArea = rTuple.pRenderWindow->getSize();
    rTuple.pBat1->SetPosition(sf::Vector2f(50.0f,vScreenArea.y/2));
    rTuple.pBat1->SetDesiredMoveDirection(eBatMoveDirection::NONE);

    rTuple.pBat2->SetDesiredMoveDirection(eBatMoveDirection::NONE);
    rTuple.pBat2->SetPosition(sf::Vector2f(vScreenArea.x-50.0f,vScreenArea.y/2) );

    rTuple.pBall->SetDesiredBallState(eBallState::ResetGamePosition);
    rTuple.pBall->SetBallVector(sf::Vector3f(vScreenArea.x/2,vScreenArea.y/2, 0.00f));
    rTuple.pBall->SetXSpeed(rTuple.pBall->GetInitialSpeed().x);
    rTuple.pBall->SetYSpeed(rTuple.pBall->GetInitialSpeed().y);
}

//----------------------------------------------------------

void GameScreen::CheckCollisions(const DataStruct rTuple)
{
    bool bIsCollidingWithP1 = isBallCollidingWithTarget(rTuple.pBall->GetShape().getGlobalBounds(), rTuple.pBat1->GetShape().getGlobalBounds());
    bool bIsCollidingWithP2 = isBallCollidingWithTarget(rTuple.pBall->GetShape().getGlobalBounds(), rTuple.pBat2->GetShape().getGlobalBounds());
    bool bIsCollidingWithWalls = isBallHittingWall(rTuple.pBall->GetShape().getGlobalBounds(), rTuple.pRenderWindow );
    bool bDidPLayerScore = isBallHittingGoal(rTuple.pBall->GetShape().getGlobalBounds(), rTuple.pRenderWindow );

    rTuple.pBall->OnBatCollision(bIsCollidingWithP1 || bIsCollidingWithP2);
    rTuple.pBall->OnWallCollision(bIsCollidingWithWalls);
    

    sf::Vector3f const ballPosition(rTuple.pBall->GetTranslationPosition().x,rTuple.pBall->GetTranslationPosition().y,100.0f);
    rTuple.pPlayer1SoundEffect->setPosition(ballPosition);
    rTuple.pPlayer2SoundEffect->setPosition(ballPosition);

    if(bDidPLayerScore)
    {
        bool isleft = rTuple.pBall->GetTranslationPosition().x < 300.0f;
        rTuple.pBall->OnScoreGoal(bDidPLayerScore, isleft);
        rTuple.pWorldGameState->SetDesiredGamestate(eGameState::GameOver);
        rTuple.pBall->StateMachine();
        ResetGame(rTuple);
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

//----------------------------------------------------------

bool GameScreen::isBallCollidingWithTarget(const sf::FloatRect box1, const sf::FloatRect box2)
{
    if( box1.intersects(box2) )
    {
        return true;
    }
    return false;
}

//----------------------------------------------------------

bool GameScreen::isBallHittingWall(const sf::FloatRect box1, sf::RenderWindow* pRenderWindow)
{
    const float rectTop = 0.0f;
    const float rectBottom = pRenderWindow->getSize().y - 15.0f;
    const float top = box1.top;
    if (top > rectBottom || top < rectTop)
    {
        return true;
    }
    return false;
}

//----------------------------------------------------------

bool GameScreen::isBallHittingGoal(const sf::FloatRect box1, sf::RenderWindow* pRenderWindow )
{
    const float fLeftGoalPosition = 0.0f;
    const float fRightGoalPosition = pRenderWindow->getSize().x;
    const float fBoxXpos = box1.left;

    if (fBoxXpos <= fLeftGoalPosition )
    {
        return true;
    }
    else if (fBoxXpos + 15 > fRightGoalPosition)
    {
        return true;
    }
    return false;
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

sf::String GameScreen::DebugTextBallState( eBallState eBallState )
{
    if (eBallState == eBallState::LEFT)
    {
        return sf::String("LEFT:");
    }
    else if (eBallState == eBallState::RIGHT)
    {
        return sf::String("RIGHT:");
    }
    else if (eBallState == eBallState::HitBall)
    {
        return sf::String("HitBall:");
    }
    else if (eBallState == eBallState::HitWall)
    {
        return sf::String("HitWall:");
    }
    else if (eBallState == eBallState::ResetGamePosition)
    {
        return sf::String("RESET:");
    }
    else
    {
        return sf::String("");
    }

    return sf::String("");
}

//----------------------------------------------------------