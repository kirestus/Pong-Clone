#include <headers/gameScreen.h>
#include <string>
#include <iostream>

//-----------------------------------------------------------------

GameScreen::GameScreen(DataStruct &rTuple)
{
    CreateGameScreen(rTuple);
    UpdateGamescreen(rTuple,m_hClock);
}

//-----------------------------------------------------------------

void GameScreen::CreateGameScreen(DataStruct& rTuple)
{
    SetScreenCenter(CalculateScreenCenter(rTuple.pRenderWindow));
    rTuple.pPlayer1SoundBuffer->loadFromFile("sfx/player1Hit.wav");
    rTuple.pPlayer2SoundBuffer->loadFromFile("sfx/player2Hit.wav");
    rTuple.pHitWallSoundBuffer->loadFromFile("sfx/wallHit.wav");
    rTuple.pScoreGoalSoundBuffer->loadFromFile("sfx/goalScored.wav");

    rTuple.pVectorFont->loadFromFile("fonts/Vector.ttf");
    rTuple.pMessage->setFont(*rTuple.pVectorFont);
    rTuple.pMessage->setFillColor(sf::Color::White);
    rTuple.pMessage->setCharacterSize(90);
    rTuple.pMessage->setOrigin(rTuple.pMessage->getLocalBounds().getSize().x/2,rTuple.pMessage->getLocalBounds().getSize().y/2);
    rTuple.pMessage->setPosition(sf::Vector2f(rTuple.fScreenWidth/2, rTuple.fScreenHeight-100.0f) ); // should set this in the middle of the screen will do later
    SetScoreText(m_aScore[0],m_aScore[1]);
    UpdateScoreText(rTuple);
}

//-----------------------------------------------------------------

sf::Vector2f GameScreen::CalculateScreenCenter(const std::shared_ptr<sf::RenderWindow> pRenderWindow)
{
    sf::Vector2f vScreenCenter;
    vScreenCenter.x = pRenderWindow->getSize().x/2;
    vScreenCenter.y = pRenderWindow->getSize().y/2;

    return vScreenCenter;
}

//-----------------------------------------------------------------

int GameScreen::UpdateGamescreen(DataStruct& rTuple, sf::Clock &rGameClock)
{
    rTuple.pGameMusic->play();
    rTuple.pGameMusic->setLoop(true);

    rTuple.pRenderWindow->setFramerateLimit(244.0);

    while (rTuple.pRenderWindow->isOpen())
    {
        const float fFrameTime = rGameClock.restart().asSeconds();
        const float fFps = 1.0 / fFrameTime;
        //std::cout << fFps << " Frames Per Second: \n";

        const bool bIsPaused = rTuple.pWorldState->GetCurrentGameState() == eGameState::Paused;

        rTuple.pWorldState->DetermineGameState();

        if (rTuple.pWorldState->GetCurrentGameState() == Boot )
        {
            ResetGame(rTuple);
            UpdateScoreText(rTuple);
        }

        rTuple.pBat1->CalculateBatSpeed(rTuple.pRenderWindow, fFrameTime, bIsPaused);
        rTuple.pBat2->CalculateBatSpeed(rTuple.pRenderWindow, fFrameTime, bIsPaused);
        rTuple.pBall->UpdateBallPosition(fFrameTime/10, bIsPaused);
        
        const eCollisionType eCollidingWith = CheckCollisions(rTuple);
        if ( eCollidingWith != eCollisionType::NoCollision )
        {
            if (GetShouldUpdateScore())
            {
                UpdateScoreText(rTuple);
            }
            HandleCollisions(rTuple, bIsPaused, eCollidingWith);
        }

        if ( rTuple.pBall->GetCurrentBallState() == eBallState::AtPlayer1 )
        {
            AttachBallToBat(rTuple.pBat1, rTuple.pBall);
            rTuple.pBall->StateMachine(rTuple.fScreenWidth);
            rTuple.pBall->SetYSpeed(rTuple.pBat1->GetVelocity()*-1.5f);
        }
        else if ( rTuple.pBall->GetCurrentBallState() == eBallState::AtPlayer2 )
        {
            AttachBallToBat(rTuple.pBat2, rTuple.pBall);
            rTuple.pBall->StateMachine(rTuple.fScreenWidth);
            rTuple.pBall->SetYSpeed(rTuple.pBat2->GetVelocity()*-1.5f );
        }

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
                command->~Command();
            }
        }

        rTuple.pRenderWindow->clear();
        rTuple.pRenderWindow->draw( *rTuple.pMessage );
        rTuple.pRenderWindow->draw( rTuple.pBat1->GetShape());
        rTuple.pRenderWindow->draw( rTuple.pBat2->GetShape());
        rTuple.pRenderWindow->draw( rTuple.pBall->ReferenceShape() );
        
        rTuple.pRenderWindow->display();
    }

    return 1;
}

//----------------------------------------------------------

void GameScreen::ResetGame(DataStruct &rTuple)
{
    if (GetisWinConditionMet())
    {
        rTuple.pWorldState->SetCurrentGamestate(eGameState::Quit);
    }

    rTuple.pBat1->SetPosition(sf::Vector2f(50.0f, rTuple.fScreenHeight/2));
    rTuple.pBat1->SetDesiredMoveDirection(eBatMoveDirection::NONE);
    rTuple.pBat1->UpdateShapeToDesiredTransform();

    rTuple.pBat2->SetPosition(sf::Vector2f(rTuple.fScreenWidth -50.0f, rTuple.fScreenHeight / 2 ));
    rTuple.pBat2->SetDesiredMoveDirection(eBatMoveDirection::NONE);
    rTuple.pBat2->UpdateShapeToDesiredTransform();

    rTuple.pBall->SetBallVector(sf::Vector3f(rTuple.fScreenWidth/2, rTuple.fScreenHeight/2, 0.00f));

    rTuple.pBall->StateMachine(rTuple.fScreenWidth);
    rTuple.pBall->SetXSpeed(rTuple.pBall->GetInitialSpeed().x);
    rTuple.pBall->SetYSpeed(rTuple.pBall->GetInitialSpeed().y);
}

//----------------------------------------------------------

eCollisionType GameScreen::CheckCollisions(DataStruct &rTuple)
{  
    const bool bIsCollidingWithP1 = isBallCollidingWithTarget(rTuple.pBall->GetShape().getGlobalBounds(), rTuple.pBat1->GetShape().getGlobalBounds());
    const bool bIsCollidingWithP2 = isBallCollidingWithTarget(rTuple.pBall->GetShape().getGlobalBounds(), rTuple.pBat2->GetShape().getGlobalBounds());
    const bool bIsCollidingWithWalls = isBallHittingWall(rTuple.pBall->GetShape().getGlobalBounds(), rTuple.pRenderWindow );
    const bool bDidPLayerScore = isBallHittingGoal(rTuple.pBall->GetShape().getGlobalBounds(),rTuple );

    //order here could matter in the rare edge case of a player hitting a wall and hitting the goal
    if ( bDidPLayerScore ){ return eCollisionType::CollisionWithGoalZone ;}
    else if ( bIsCollidingWithP1 ){ return eCollisionType::CollisionWithPlayer1 ;}
    else if ( bIsCollidingWithP2 ){ return eCollisionType::CollisionWithPlayer2 ;}
    else if ( bIsCollidingWithWalls ){ return eCollisionType::CollisionWithWall ;}
    return eCollisionType::NoCollision ;
}

//----------------------------------------------------------

void GameScreen::HandleCollisions(DataStruct &rTuple, const bool bIsPaused, const eCollisionType eCollidingwith )
{
    if (eCollidingwith == eCollisionType::CollisionWithGoalZone)
    {
        const bool isleft = rTuple.pBall->GetTranslationPosition().x < rTuple.fScreenWidth/2;
        rTuple.pBall->OnScoreGoal(true, isleft, rTuple.fScreenWidth);

        if ( GetisWinConditionMet() )
        {
            rTuple.pWorldState->SetDesiredGamestate(eGameState::GameOver);
        }

        ResetGame(rTuple);
    }
    else if (eCollidingwith == eCollisionType::CollisionWithPlayer1||
        eCollidingwith == eCollisionType::CollisionWithPlayer2)
    {
        const bool bIsCollidingWithP1 = eCollidingwith == eCollisionType::CollisionWithPlayer1 ? true : false; 
        
        std::shared_ptr<Bat> pBat = bIsCollidingWithP1 ? rTuple.pBat1 : rTuple.pBat2 ;
        const bool bIsBallOnLeft = (rTuple.pBall->GetTranslationPosition().x < rTuple.fScreenWidth/2);
        if ( bIsCollidingWithP1 && bIsBallOnLeft || !bIsCollidingWithP1 && !bIsBallOnLeft )
        {
            const eBallState eBallGoingDir = bIsCollidingWithP1 ? RIGHT : LEFT;
            sf::Vector3f const ballPosition(rTuple.pBall->GetTranslationPosition().x,rTuple.pBall->GetTranslationPosition().y,100.0f);
            rTuple.pBall->OnBatCollision(rTuple.fScreenHeight);
            rTuple.pBall->SetYSpeed(pBat->GetVelocity() + rTuple.pBall->GetYSpeed());
            rTuple.pBall->SetDesiredBallState(eBallGoingDir);
            rTuple.pBall->StateMachine(rTuple.fScreenWidth);
            sf::Sound* pPlayThisSound = eBallGoingDir != LEFT ? rTuple.pPlayer1SoundEffect :rTuple.pPlayer2SoundEffect;
            pPlayThisSound->setPosition(ballPosition);
            pPlayThisSound->play();
        }
    }
    else if(eCollidingwith == eCollisionType::CollisionWithWall)
    {
        rTuple.pBall->OnWallCollision(true, rTuple.fScreenHeight);
        sf::Sound* pSound = rTuple.pHitWallSoundEffect;
        pSound->play();
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

bool GameScreen::isBallHittingWall(const sf::FloatRect box1, const std::shared_ptr<sf::RenderWindow> pRenderWindow)
{
    const float rectTop = 0.0f;
    const float rectBottom = pRenderWindow->getSize().y - 15.0f;
    if (box1.top > rectBottom || box1.top < rectTop)
    {
        return true;
    }
    return false;
}

//----------------------------------------------------------

bool GameScreen::isBallHittingGoal(const sf::FloatRect box1, DataStruct &rTuple )
{
    const float fLeftGoalPosition = 0.0f;
    const float fRightGoalPosition = rTuple.fScreenWidth;
    const float fBoxXpos = box1.left;
    sf::Sound* pSound = rTuple.pScoreGoalSoundEffect;

    if (fBoxXpos < fLeftGoalPosition )
    {
        pSound->play();
        m_aScore[1] ++;

        return true;
    }
    else if (fBoxXpos + 15 > fRightGoalPosition)
    {
        pSound->play();        
        m_aScore[0] ++;
        
        return true;
    }
    return false;
}

//----------------------------------------------------------

std::string GameScreen::SetScoreText(const int &iPlayer1Score, const int &iPlayer2Score)
{
   return std::to_string(iPlayer1Score)+" - "+ std::to_string(iPlayer2Score);
}

//----------------------------------------------------------

void GameScreen::UpdateScoreText(DataStruct& rTuple)
{
    std::string scoreString = GameScreen::SetScoreText(m_aScore[0],m_aScore[1]);
    rTuple.pMessage->setString(sf::String(scoreString));
    rTuple.pMessage->setOrigin(rTuple.pMessage->getLocalBounds().getSize().x/2,rTuple.pMessage->getLocalBounds().getSize().y/2);
}

//------------------------------------------------------------

void GameScreen::AttachBallToBat(std::shared_ptr<Bat> pBat, std::shared_ptr<Ball> pBall)
{
    const sf::Vector2f vBatPosition = pBat->GetShape().getPosition();
    const float fOffset = pBat->GetPlayerNumber() == ePlayerNumber::PLAYER1  ? 20.0f : -20.0f;
    pBall->SetBallVector(sf::Vector3f( vBatPosition.x + fOffset, vBatPosition.y, 0.0f));
}
