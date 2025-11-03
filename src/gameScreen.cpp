#include <headers/gameScreen.h>
#include <string>
#include <debugText.cpp>
#include <iostream>

//-----------------------------------------------------------------

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
    //wololo.play();
    wololo.setLoop(true);

    rTuple.pRenderWindow->setFramerateLimit(244.0);
    Bat* pBat1 = rTuple.pBat1;
    Bat* pBat2 = rTuple.pBat2;
    Ball* pBall = rTuple.pBall;

    while (rTuple.pRenderWindow->isOpen())
    {
        float fFrameTime = rGameClock.restart().asSeconds();
        float fFps = 1.0 / fFrameTime;
        std::cout << fFps << " Frames Per Second: \n";

        //rTuple.pMessage->setString(DebugText::DebugTextGameState(rTuple.pWorldGameState->GetCurrentGameState()));
        //rTuple.pMessage->setString(DebugText::DebugTextBallState(rTuple.pBall->GetCurrentBallState()));
        bool bIsPaused = rTuple.pWorldGameState->GetCurrentGameState() == eGameState::Paused;
        rTuple.pWorldGameState->DetermineGameState();

        if (rTuple.pWorldGameState->GetCurrentGameState() == Boot )
        {
            ResetGame(rTuple);
            UpdateScoreText(rTuple);
        }

        pBat1->CalculateBatSpeed(rTuple.pRenderWindow, fFrameTime, bIsPaused);
        pBat2->CalculateBatSpeed(rTuple.pRenderWindow, fFrameTime, bIsPaused);
        pBall->UpdateBallPosition(fFrameTime/10, bIsPaused);
        
        eCollisionType eCollidingWith = CheckCollisions(rTuple);
        if ( eCollidingWith != eCollisionType::NoCollision )
        {
            UpdateScoreText(rTuple);
            HandleCollisions(rTuple, bIsPaused, eCollidingWith);
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
        rTuple.pRenderWindow->draw( pBat1->GetShape());
        rTuple.pRenderWindow->draw( pBat2->GetShape());
        rTuple.pRenderWindow->draw( pBall->ReferenceShape() );
        
        rTuple.pRenderWindow->display();
    }

    return 1;
}

//----------------------------------------------------------

void GameScreen::ResetGame(DataStruct &rTuple)
{
    rTuple.pBat1->SetPosition(sf::Vector2f(50.0f,*rTuple.fScreenHeight/2));
    rTuple.pBat1->SetDesiredMoveDirection(eBatMoveDirection::NONE);

    rTuple.pBat2->SetDesiredMoveDirection(eBatMoveDirection::NONE);
    rTuple.pBat2->SetPosition(sf::Vector2f(*rTuple.fScreenWidth -50.0f,*rTuple.fScreenWidth / 2) );

    rTuple.pBall->SetBallVector(sf::Vector3f(*rTuple.fScreenWidth/2,*rTuple.fScreenHeight/2, 0.00f));

    rTuple.pBall->StateMachine(*rTuple.fScreenWidth);
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
        const bool isleft = rTuple.pBall->GetTranslationPosition().x < *rTuple.fScreenWidth/2;
        rTuple.pBall->OnScoreGoal(true, isleft, *rTuple.fScreenWidth);
        rTuple.pWorldGameState->SetDesiredGamestate(eGameState::GameOver);
        ResetGame(rTuple);
        return;
    }
    else if (eCollidingwith == eCollisionType::CollisionWithPlayer1||
        eCollidingwith == eCollisionType::CollisionWithPlayer2)
    {
        const bool bIsCollidingWithP1 = eCollidingwith == eCollisionType::CollisionWithPlayer1 ? true : false; 
        const Bat* pBat = bIsCollidingWithP1 ? rTuple.pBat1 : rTuple.pBat2 ;
        const eBallState eBallGoingDir = bIsCollidingWithP1 ? RIGHT : LEFT;

        sf::Vector3f const ballPosition(rTuple.pBall->GetTranslationPosition().x,rTuple.pBall->GetTranslationPosition().y,100.0f);
        rTuple.pPlayer1SoundEffect->setPosition(ballPosition);
        rTuple.pPlayer2SoundEffect->setPosition(ballPosition);

        rTuple.pBall->OnBatCollision(*rTuple.fScreenHeight);
        
        rTuple.pBall->SetYSpeed(pBat->GetVelocity() + rTuple.pBall->GetYSpeed());
        rTuple.pBall->SetDesiredBallState(eBallGoingDir);
        rTuple.pBall->StateMachine(*rTuple.fScreenWidth);
        sf::Sound* pPlayThisSound = eBallGoingDir != LEFT ? rTuple.pPlayer1SoundEffect :rTuple.pPlayer2SoundEffect;
        pPlayThisSound->play();
        return;
    }
    else if(eCollidingwith == eCollisionType::CollisionWithWall)
    {
        rTuple.pBall->OnWallCollision(true, *rTuple.fScreenHeight);
        return;
    }
return;
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

bool GameScreen::isBallHittingGoal(const sf::FloatRect box1, DataStruct &rTuple )
{
    const float fLeftGoalPosition = 0.0f;
    const float fRightGoalPosition = *rTuple.fScreenWidth;
    const float fBoxXpos = box1.left;

    if (fBoxXpos <= fLeftGoalPosition  > 0.1f )
    {
        m_aScore[1] ++;
        
        UpdateScoreText(rTuple);
        return true;
    }
    else if (fBoxXpos + 15 > fRightGoalPosition)
    {        
        m_aScore[0] ++;
        UpdateScoreText(rTuple);
        return true;
    }
    return false;
}

//----------------------------------------------------------

std::string GameScreen::SetScoreText(int &iPlayer1Score, int &iPlayer2Score)
{
   return std::to_string(iPlayer1Score)+" - "+ std::to_string(iPlayer2Score);
}

//----------------------------------------------------------

void GameScreen::UpdateScoreText(DataStruct& rTuple)
{
    sf::Text* pText = rTuple.pMessage;
    std::string scoreString = GameScreen::SetScoreText(m_aScore[0],m_aScore[1]);
    pText->setString(sf::String(scoreString));
    pText->setOrigin(pText->getLocalBounds().getSize().x/2,pText->getLocalBounds().getSize().y/2);

}