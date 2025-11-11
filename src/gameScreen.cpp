#include <headers/gameScreen.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>

//forward declaring
static float CreateRandomAngle(int minRange, int maxRange);

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
    rTuple.pScoreGoalSoundBuffer->loadFromFile("sfx/goalScored2.wav");
    rTuple.pYouWinSoundBuffer->loadFromFile("sfx/goalScored.wav");
    rTuple.pSpitBallSoundBuffer->loadFromFile("sfx/spitBall.wav");

    rTuple.pVectorFont->loadFromFile("fonts/Vector.ttf");
    rTuple.pMessageText->setFont(*rTuple.pVectorFont);
    rTuple.pMessageText->setFillColor(sf::Color::White);
    rTuple.pMessageText->setOrigin(rTuple.pMessageText->getLocalBounds().getSize().x/2,rTuple.pMessageText->getLocalBounds().getSize().y/2);
    rTuple.pMessageText->setPosition(sf::Vector2f(rTuple.fScreenWidth/2, rTuple.fScreenHeight-100.0f) ); // should set this in the middle of the screen will do later
    
    rTuple.pScoreText->setFont(*rTuple.pVectorFont);
    rTuple.pScoreText->setFillColor(sf::Color(255,255,255,200));
    rTuple.pScoreText->setOrigin(rTuple.pScoreText->getLocalBounds().getSize().x/2,rTuple.pScoreText->getLocalBounds().getSize().y/2);
    rTuple.pScoreText->setPosition(sf::Vector2f(rTuple.fScreenWidth/2, 50.0f) ); // should set this in the middle of the screen will do later
    rTuple.pRenderWindow->setView(*rTuple.pView);
    
    SetScoreText(m_aScore[0],m_aScore[1]);
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
    rTuple.pGameMusic->pause();
    rTuple.pGameMusic->setLoop(true);
    srand(time(0));
    rTuple.pBall->SetYSpeed(CreateRandomAngle(-2000.0f,2000.0f));

    rTuple.pRenderWindow->setFramerateLimit(244.0);
    rTuple.pRenderWindow->setMouseCursorVisible(false);

    while (rTuple.pRenderWindow->isOpen())
    {
        const float fFrameTime = rGameClock.restart().asSeconds();
        const float fFps = 1.0 / fFrameTime;
        //std::cout << fFps << " Frames Per Second: \n";

        bool bIsPaused = rTuple.pWorldState->GetCurrentGameState() == eGameState::Paused;

        rTuple.pWorldState->DetermineGameState();

        if (rTuple.pWorldState->GetCurrentGameState() == Boot )
        {
            ResetGame(rTuple);
        }

        rTuple.pBat1->CalculateBatSpeed(rTuple.pRenderWindow, fFrameTime, bIsPaused);
        rTuple.pBat2->CalculateBatSpeed(rTuple.pRenderWindow, fFrameTime, bIsPaused);
        rTuple.pBall->UpdateBallPosition(fFrameTime/10, bIsPaused);
        
        const eCollisionType eCollidingWith = CheckCollisions(rTuple);
        if ( eCollidingWith != eCollisionType::NoCollision )
        {
            HandleCollisions(rTuple, bIsPaused, eCollidingWith);
        }

        //included here because i want to check for none type also
        SetLastCollisionType(eCollidingWith);
        ShakeScreen(rTuple,0.5f, eCollidingWith);

        if ( rTuple.pBall->GetCurrentBallState() == eBallState::AtPlayer1 )
        {
            AttachBallToBat(rTuple.pBat1, rTuple.pBall);
            rTuple.pBall->StateMachine(rTuple.fScreenWidth);
            rTuple.pBall->SetYSpeed(rTuple.pBat1->GetVelocity()*-1.9f);
        }
        else if ( rTuple.pBall->GetCurrentBallState() == eBallState::AtPlayer2 )
        {
            AttachBallToBat(rTuple.pBat2, rTuple.pBall);
            rTuple.pBall->StateMachine(rTuple.fScreenWidth);
            rTuple.pBall->SetYSpeed(rTuple.pBat2->GetVelocity()*-1.9f );
        }
        
        UpdateUIText( GetisWinConditionMet(), bIsPaused, rTuple );
        UpdateScoreText(rTuple);

        sf::Event event;
        while (rTuple.pRenderWindow->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                rTuple.pRenderWindow->close();
            }
            Command* command = m_hInputHandler.HandleInput( &event, GetisWinConditionMet());
            if (command) 
            {  
                command->execute(rTuple);
            }
        }

        rTuple.pRenderWindow->clear();
        rTuple.pRenderWindow->draw( *rTuple.pMessageText );
        rTuple.pRenderWindow->draw( *rTuple.pScoreText );
        rTuple.pRenderWindow->draw( rTuple.pBat1->GetShape());
        rTuple.pRenderWindow->draw( rTuple.pBat2->GetShape());
        rTuple.pRenderWindow->draw( rTuple.pBall->ReferenceShape() );
        
        rTuple.pRenderWindow->display();
        m_lDetermFrame ++;
    }

    return 1;
}

//----------------------------------------------------------

void GameScreen::ResetGame(DataStruct &rTuple)
{
    rTuple.pBat1->SetPosition(sf::Vector2f(50.0f, rTuple.fScreenHeight/2));
    rTuple.pBat1->SetDesiredMoveDirection(eBatMoveDirection::NONE);
    rTuple.pBat1->UpdateShapeToDesiredTransform();

    rTuple.pBat2->SetPosition(sf::Vector2f(rTuple.fScreenWidth -50.0f, rTuple.fScreenHeight / 2 ));
    rTuple.pBat2->SetDesiredMoveDirection(eBatMoveDirection::NONE);
    rTuple.pBat2->UpdateShapeToDesiredTransform();

    rTuple.pBall->SetBallVector(sf::Vector3f(rTuple.fScreenWidth/2, rTuple.fScreenHeight/2, 0.00f));
    rTuple.pBall->StateMachine(rTuple.fScreenWidth);
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
            if (rTuple.pWorldState->GetShouldPlaySFX())
            {
                rTuple.pYouWinSoundEffect->play();  
            }

            rTuple.pWorldState->SetDesiredGamestate(eGameState::GameOver);
        }
        else if (rTuple.pWorldState->GetShouldPlaySFX())
        {
            rTuple.pScoreGoalSoundEffect->play();
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
            rTuple.pBall->SetYSpeed(pBat->GetVelocity() + rTuple.pBall->GetYSpeed()+CreateRandomAngle(-500.0f,500.0f));
            rTuple.pBall->SetDesiredBallState(eBallGoingDir);
            rTuple.pBall->StateMachine(rTuple.fScreenWidth);
            
            sf::Sound* pPlayThisSound = eBallGoingDir != LEFT ? rTuple.pPlayer1SoundEffect :rTuple.pPlayer2SoundEffect;
            pPlayThisSound->setPosition(ballPosition);
            if (rTuple.pWorldState->GetShouldPlaySFX())
            {
                pPlayThisSound->play();
            }
        }
    }
    else if(eCollidingwith == eCollisionType::CollisionWithWall && 
        GetLastCollisionType()!=eCollisionType::CollisionWithWall)
    {
        rTuple.pBall->OnWallCollision(true, rTuple.fScreenWidth);
        if (rTuple.pWorldState->GetShouldPlaySFX())
        {
            float fPitchShift = 0.8 + (abs(rTuple.pBall->GetYSpeed()/30000));
            rTuple.pHitWallSoundEffect->setVolume(abs(rTuple.pBall->GetYSpeed()/100));
            rTuple.pHitWallSoundEffect->setPitch(fPitchShift);
            rTuple.pHitWallSoundEffect->play();
        }
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
    if (box1.top >= rectBottom || box1.top <= rectTop)
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

    if (fBoxXpos < fLeftGoalPosition )
    {
        m_aScore[1] ++;

        return true;
    }
    else if (fBoxXpos + 15 > fRightGoalPosition)
    {   
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
    rTuple.pScoreText->setString(sf::String(scoreString));
    rTuple.pScoreText->setCharacterSize(70);
    rTuple.pScoreText->setOrigin(rTuple.pScoreText->getLocalBounds().getSize().x/2,rTuple.pScoreText->getLocalBounds().getSize().y/2);
}

void GameScreen::UpdateHudText(DataStruct& rTuple, std::string sDesiredText)
{
    rTuple.pMessageText->setString(sf::String(sDesiredText));
    rTuple.pMessageText->setOrigin(rTuple.pMessageText->getLocalBounds().getSize().x/2,rTuple.pMessageText->getLocalBounds().getSize().y/2);
    rTuple.pMessageText->setCharacterSize(50);
}

//------------------------------------------------------------

void GameScreen::AttachBallToBat(std::shared_ptr<Bat> pBat, std::shared_ptr<Ball> pBall)
{
    const sf::Vector2f vBatPosition = pBat->GetShape().getPosition();
    const float fOffset = pBat->GetPlayerNumber() == ePlayerNumber::PLAYER1  ? 20.0f : -20.0f;
    pBall->SetBallVector(sf::Vector3f( vBatPosition.x + fOffset, vBatPosition.y, 0.0f));
}

void GameScreen::UpdateUIText(bool bIsGameOver, bool bIsPaused, DataStruct& rTuple)
{
    sf::Color fadeInColor(255,255,255,GetTextFadeTimer().getElapsedTime().asSeconds()*100);

    if (GetTextFadeTimer().getElapsedTime().asSeconds()*100 < 225 ) 
    {
        rTuple.pMessageText->setFillColor(sf::Color(255,255,255,m_TextFadeOutTimer.getElapsedTime().asSeconds()*100));
    }

    if (bIsGameOver && rTuple.pWorldState->GetLastGoalScoredOnP1())
    {
        UpdateHudText(rTuple,std::string("PLAYER 2 WINS!:"));
    }
    else if (bIsGameOver && !rTuple.pWorldState->GetLastGoalScoredOnP1())
    {
        UpdateHudText(rTuple,std::string("PLAYER 1 WINS!:"));
    }
    else if (bIsPaused)
    {
        UpdateHudText(rTuple,std::string("SPACE TO UNPAUSE:"));
    }
    else if (rTuple.pBall->GetCurrentBallState() == AtPlayer1)
    {
        UpdateHudText(rTuple,"PRESS F TO SERVE:");
        UpdateScoreText(rTuple);
    }
    else if (rTuple.pBall->GetCurrentBallState() == AtPlayer2 )
    {
        UpdateHudText(rTuple,"PRESS / TO SERVE:");
        UpdateScoreText(rTuple);
    }
    else if (!rTuple.pWorldState->GetShouldPlaySFX())
    {
        UpdateHudText(rTuple,"SOUND MUTED:");
        UpdateScoreText(rTuple);
    }
    else
    {
        //Running this here because it will reset every update unless in the other states
        m_TextFadeOutTimer.restart();
        rTuple.pMessageText->setFillColor(sf::Color(255,255,255,0));
    }
}

//------------------------------------------------------------

static float CreateRandomAngle(int minRange, int maxRange)
{
    return rand() % (maxRange - minRange + 1 ) + minRange;
}

//------------------------------------------------------------

void GameScreen::ShakeScreen(DataStruct &rTuple, const float fMagnitude, eCollisionType eJustHit )
{
    if (eJustHit == CollisionWithWall)
    {
        m_lLastShakeFrame = m_lDetermFrame;
        rTuple.pView->move(sf::Vector2f(0.0f,fMagnitude*rTuple.pBall->GetYSpeed()/400));
    }
    else if ( eJustHit == CollisionWithPlayer1 || eJustHit == CollisionWithPlayer2 )
    {
        m_lLastShakeFrame = m_lDetermFrame;
        rTuple.pView->move(sf::Vector2f(fMagnitude*rTuple.pBall->GetXSpeed()/1200,0.0f));
    }
    else if (m_lLastShakeFrame + 15 <= m_lDetermFrame)
    {
         rTuple.pView->setCenter(sf::Vector2f(rTuple.fScreenWidth/2,rTuple.fScreenHeight/2));
    }
    rTuple.pRenderWindow->setView(*rTuple.pView);
}
