#include <headers/gameScreen.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>

//forward declaring
static float CreateRandomAngle(const int minRange, const int maxRange);

//-----------------------------------------------------------------

GameScreen::GameScreen(const DataStruct &rTuple)
{
    CreateGameScreen(rTuple);
    UpdateGamescreen(rTuple,m_hClock);
}

//-----------------------------------------------------------------

void GameScreen::CreateGameScreen(const DataStruct& rTuple)
{
    //Set values that dont need to be updated each cycle.
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
    CreateMiddleLine(rTuple);
    SetScoreText(m_aScore[0],m_aScore[1]);

    SetBoundryEdgeShapes(rTuple);
    srand(time(0));
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

int GameScreen::UpdateGamescreen(const DataStruct& rTuple, sf::Clock &rGameClock)
{
    rTuple.pGameMusic->pause();
    rTuple.pGameMusic->setLoop(true);
    rTuple.pBall->SetYSpeed(CreateRandomAngle(-2000.0f,2000.0f));

    rTuple.pRenderWindow->setFramerateLimit(244.0);
    rTuple.pRenderWindow->setMouseCursorVisible(false);

    while (rTuple.pRenderWindow->isOpen())
    {
        const int iSimFrame = rTuple.pWorldState->GetCurrentSimFrame();
        const float fFrameTime = rGameClock.restart().asSeconds();
        const float fFps = 1.0 / fFrameTime;
        //std::cout << fFps << " Frames Per Second: \n";

        const bool bIsPaused = rTuple.pWorldState->GetCurrentGameState() == eGameState::Paused;

        rTuple.pWorldState->DetermineGameState();

        if (rTuple.pWorldState->GetCurrentGameState() == eGameState::Boot )
        {
            ResetGame(rTuple);
        }
        
        rTuple.pBat1->CalculateBatSpeed(rTuple.pRenderWindow, fFrameTime, bIsPaused);
        rTuple.pBat2->CalculateBatSpeed(rTuple.pRenderWindow, fFrameTime, bIsPaused);
        rTuple.pBall->UpdateBallPosition(fFrameTime/10, bIsPaused);
        
        const eCollisionType eCollidingWith = CheckCollisions(rTuple, bIsPaused);
        if ( eCollidingWith != eCollisionType::NoCollision )
        {
            HandleCollisions(rTuple, bIsPaused, eCollidingWith, iSimFrame);
        }

        SetLastCollisionType(eCollidingWith);

        ShakeScreen(rTuple,0.5f, eCollidingWith, bIsPaused);

        if (ShouldAttachBallToBat(rTuple))
        {
            if ( rTuple.pBall->GetCurrentBallState() == eBallState::AtPlayer1 )
            {
                AttachBallToBat(rTuple.pBat1, rTuple.pBall, rTuple.fScreenWidth);
            }
            else
            {
                AttachBallToBat(rTuple.pBat2, rTuple.pBall, rTuple.fScreenWidth);
            }
        }

        const bool bUpdatedUiText = UpdateUIText( GetisWinConditionMet(), bIsPaused, rTuple );

        DimMiddleLine(rTuple, bUpdatedUiText);
        UpdateScoreText(rTuple, iSimFrame, bIsPaused);

        rTuple.pBat1->UpdateHitVFX(rTuple.pRenderWindow, iSimFrame, rTuple.pBall->GetShape().getPosition().y);
        rTuple.pBat2->UpdateHitVFX(rTuple.pRenderWindow, iSimFrame, rTuple.pBall->GetShape().getPosition().y);

        //UpdateWallBounceVFX(rTuple); TODO: Get this working later

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

        // This Section handles all the rendering for the game

        rTuple.pRenderWindow->clear();

        for(int i = m_iNumberOfLines; i>=0 ; i--)
        {
            rTuple.pRenderWindow->draw( m_DashedLineRect[i] );
        }

        rTuple.pRenderWindow->draw( *rTuple.pMessageText );
        rTuple.pRenderWindow->draw( *rTuple.pScoreText );

        if (rTuple.pBall->GetCurrentBallState() != eBallState::AtPlayer1 && 
        rTuple.pBall->GetCurrentBallState() != eBallState::AtPlayer2 )
        {
            for (int i = rTuple.pBall->GetTrailShapeArrayLength(); i >= 0; i --)
            {
                rTuple.pRenderWindow->draw( rTuple.pBall->GetTrailShapeArray()[i] );
            }
        }
        
        //TODO: Get the vfx for the edge bounce working better
        /*
        for(int i = m_iBounceVFXArrayLength; i >= 0; i --)
        {
            if(rTuple.pWorldState->GetDidBallLastHitScreenTop())
            {
                rTuple.pRenderWindow->draw(m_sTopEdge[i]);
            }
            else
            {
                rTuple.pRenderWindow->draw(m_sBottomEdge[i]);
            }
        }
        */

        for (int i = rTuple.pBat1->GetBatVFXArrayLength(); i >= 0; i --)
        {
            rTuple.pRenderWindow->draw( rTuple.pBat1->GetBatVFXShapeArray()[i]);
            rTuple.pRenderWindow->draw( rTuple.pBat2->GetBatVFXShapeArray()[i]);
        }

        rTuple.pRenderWindow->draw( rTuple.pBat1->GetShape());
        rTuple.pRenderWindow->draw( rTuple.pBat2->GetShape());

        rTuple.pRenderWindow->draw( rTuple.pBall->ReferenceShape() );
        
        rTuple.pRenderWindow->display();
        if(!bIsPaused)
        {
            rTuple.pBall->UpdateBallTrail( iSimFrame );
            rTuple.pWorldState->IncrimentSimFrame();
        }
    }

    return 1;
}

//----------------------------------------------------------

void GameScreen::ResetGame(const DataStruct &rTuple)
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

eCollisionType GameScreen::CheckCollisions(const DataStruct &rTuple,const bool isPaused)
{  
    const sf::FloatRect pBallHitbox = rTuple.pBall->GetShape().getGlobalBounds();
    
    const bool bIsCollidingWithP1 = isBallCollidingWithTarget(pBallHitbox, rTuple.pBat1->GetShape().getGlobalBounds());
    const bool bIsCollidingWithP2 = isBallCollidingWithTarget(pBallHitbox, rTuple.pBat2->GetShape().getGlobalBounds());
    const bool bIsCollidingWithWalls = isBallHittingWall(pBallHitbox, rTuple.pRenderWindow );
    const bool bDidPLayerScore = isBallHittingGoal(pBallHitbox,rTuple, isPaused );

    //order here could matter in the rare edge case of a player hitting a wall and hitting the goal
    if ( bDidPLayerScore )
    { 
        return eCollisionType::CollisionWithGoalZone ;
    }
    else if ( bIsCollidingWithP1 )
    { 
        return eCollisionType::CollisionWithPlayer1 ;
    }
    else if ( bIsCollidingWithP2 )
    { 
        return eCollisionType::CollisionWithPlayer2 ;
    }
    else if ( bIsCollidingWithWalls )
    { 
        return eCollisionType::CollisionWithWall ;
    }

    return eCollisionType::NoCollision ;
}

//----------------------------------------------------------

void GameScreen::HandleCollisions(const DataStruct &rTuple, const bool bIsPaused, const eCollisionType eCollidingwith,const int iSimFrame )
{
    if (eCollidingwith == eCollisionType::CollisionWithGoalZone)
    {
        const bool isleft = rTuple.pBall->GetTranslationPosition().x < rTuple.fScreenWidth/2;
        rTuple.pBall->OnScoreGoal(true, isleft, rTuple.fScreenWidth);
        m_lLastGoalScoredFrame = iSimFrame;
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
    else if (eCollidingwith == eCollisionType::CollisionWithPlayer1 
            || eCollidingwith == eCollisionType::CollisionWithPlayer2)
    {
        const bool bIsCollidingWithP1 = eCollidingwith == eCollisionType::CollisionWithPlayer1 ? true : false; 
        const std::shared_ptr<Bat> pBat = bIsCollidingWithP1 ? rTuple.pBat1 : rTuple.pBat2 ;
        const bool bIsBallOnLeft = (rTuple.pBall->GetTranslationPosition().x < rTuple.fScreenWidth/2);
        if ( bIsCollidingWithP1 && bIsBallOnLeft || !bIsCollidingWithP1 && !bIsBallOnLeft )
        {
            const eBallState eBallGoingDir = bIsCollidingWithP1 ? eBallState::RIGHT : eBallState::LEFT;
            sf::Vector3f const ballPosition(rTuple.pBall->GetTranslationPosition().x,rTuple.pBall->GetTranslationPosition().y,100.0f);
            
            rTuple.pBall->OnBatCollision(rTuple.fScreenHeight);
            if(pBat->GetLastHitFrame() +10 < iSimFrame ) // checking against edge cases where you clip the ball
            {
                rTuple.pBall->SetYSpeed(pBat->GetVelocity() + rTuple.pBall->GetYSpeed()+CreateRandomAngle(-500.0f,500.0f)); 
                rTuple.pBall->SetDesiredBallState(eBallGoingDir);
                rTuple.pBall->StateMachine(rTuple.fScreenWidth);

                if(eBallGoingDir != eBallState::LEFT)
                {
                    rTuple.pBat1->SetLastHitFrame(iSimFrame);
                }
                else
                {
                    rTuple.pBat2->SetLastHitFrame(iSimFrame);
                }
            }
            
            sf::Sound* pPlayThisSound = eBallGoingDir != eBallState::LEFT ? rTuple.pPlayer1SoundEffect :rTuple.pPlayer2SoundEffect;
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

        bool bHitTop =  rTuple.pBall->GetDesiredBallState() == eBallState::HitTopWall;
        rTuple.pWorldState->SetBallLastHitTop(bHitTop);

        if (rTuple.pWorldState->GetShouldPlaySFX())
        {
            const float fPitchShift = 0.8 + (abs(rTuple.pBall->GetYSpeed()/30000));
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

bool GameScreen::isBallHittingGoal(const sf::FloatRect box1,const DataStruct &rTuple,const bool isPaused )
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
   return std::to_string(iPlayer1Score)+"-"+ std::to_string(iPlayer2Score);
}

//----------------------------------------------------------

void GameScreen::UpdateScoreText(const DataStruct& rTuple, const int iSimFrame, const bool bIsPaused)
{
    if (bIsPaused)
    {
        return;
    }
    const std::string scoreString = GameScreen::SetScoreText(m_aScore[0],m_aScore[1]);
    rTuple.pScoreText->setString(sf::String(scoreString));
    if ( m_lLastGoalScoredFrame+30 >= rTuple.pWorldState->GetCurrentSimFrame() )
    {
        rTuple.pScoreText->scale(sf::Vector2f(1.0025,1.0025));
        rTuple.pScoreText->setFillColor(sf::Color(255,255,255,255));
    }
    else
    {
        rTuple.pScoreText->setScale(sf::Vector2f(1.0,1.0));
       rTuple.pScoreText->setCharacterSize(80.0f); 
       rTuple.pScoreText->setFillColor(sf::Color(255,255,255,150));
       rTuple.pScoreText->setLetterSpacing(0.4);
    }
    rTuple.pScoreText->setOrigin(rTuple.pScoreText->getLocalBounds().getSize().x/2,rTuple.pScoreText->getLocalBounds().getSize().y/2);
}

//------------------------------------------------------------

void GameScreen::UpdateHudText(const DataStruct& rTuple, const std::string sDesiredText)
{
    rTuple.pMessageText->setString(sf::String(sDesiredText));
    rTuple.pMessageText->setOrigin(rTuple.pMessageText->getLocalBounds().getSize().x/2,rTuple.pMessageText->getLocalBounds().getSize().y/2);
    rTuple.pMessageText->setCharacterSize(50);
}

//------------------------------------------------------------

void GameScreen::AttachBallToBat(const std::shared_ptr<Bat> pBat, const std::shared_ptr<Ball> pBall, const float fScreenWidth)
{
    pBall->StateMachine(fScreenWidth);
    pBall->SetYSpeed(pBat->GetVelocity()*-1.9f);

    const sf::Vector2f vBatPosition = pBat->GetShape().getPosition();
    const float fOffset = pBat->GetPlayerNumber() == ePlayerNumber::PLAYER1  ? 20.0f : -20.0f;
    pBall->SetBallVector(sf::Vector3f( vBatPosition.x + fOffset, vBatPosition.y, 0.0f));
}

//------------------------------------------------------------

bool GameScreen::UpdateUIText(const bool bIsGameOver, const bool bIsPaused, const DataStruct& rTuple )
{
    sf::Color fadeInColor(255,255,255,GetTextFadeTimer().getElapsedTime().asSeconds()*100);

    const bool bIsDebugging = rTuple.pDebugText->GetIsDebuggingGameState() || rTuple.pDebugText->GetIsDebuggingBallState();

    if(bIsDebugging)
    {
        if(rTuple.pDebugText->GetIsDebuggingGameState())
        {
            UpdateHudText(rTuple,std::string(rTuple.pDebugText->DebugTextGameState(rTuple.pWorldState->GetCurrentGameState())));

        }
        else
        {
            UpdateHudText(rTuple,std::string(rTuple.pDebugText->DebugTextBallState(rTuple.pBall->GetCurrentBallState())));
        }
    }
    else
    {
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
        else if (rTuple.pBall->GetCurrentBallState() == eBallState::AtPlayer1)
        {
            UpdateHudText(rTuple,"PRESS F TO SERVE:");
        }
        else if (rTuple.pBall->GetCurrentBallState() == eBallState::AtPlayer2 )
        {
            UpdateHudText(rTuple,"PRESS / TO SERVE:");
        }
        else if (!rTuple.pWorldState->GetShouldPlaySFX())
        {
            UpdateHudText(rTuple,"SOUND MUTED:");
        }
        else
        {

            m_TextFadeOutTimer.restart();
            rTuple.pMessageText->setFillColor(sf::Color(255,255,255,0));
            return false;
        }
    }
    
    return true;
}

//------------------------------------------------------------

static float CreateRandomAngle(const int minRange, const int maxRange)
{
    return rand() % (maxRange - minRange + 1 ) + minRange;
}

//------------------------------------------------------------

void GameScreen::ShakeScreen(const DataStruct &rTuple, const float fMagnitude, const eCollisionType eJustHit, const bool isPaused )
{
    const int iSimFrame = rTuple.pWorldState->GetCurrentSimFrame();
    sf::Vector2f vSlamForce;
    static constexpr int iTotalSlamFrames = 20;
    static const sf::Vector2 vForceScalingRatio(2,1);
    static constexpr float fForceScale = 550;
    static const sf::Vector2f vScaledForces(vForceScalingRatio.x*fForceScale,vForceScalingRatio.y*fForceScale);

    if (isPaused)
    {
        return;
    }

    if (eJustHit == eCollisionType::CollisionWithWall)
    {
        SetSimFrameTopLastHit(iSimFrame);
        m_iLastShakeFrame = iSimFrame;
        vSlamForce = sf::Vector2f(0.0f,fMagnitude*rTuple.pBall->GetYSpeed()/vScaledForces.y);
        SetBoundryEdgeShapes(rTuple);
    }
    else if ( eJustHit == eCollisionType::CollisionWithPlayer1 
            || eJustHit == eCollisionType::CollisionWithPlayer2 )
    {
        m_iLastShakeFrame = iSimFrame;
        vSlamForce = sf::Vector2f(fMagnitude*rTuple.pBall->GetXSpeed()/vScaledForces.x,0.0f);
    }

    if( m_iLastShakeFrame + (iTotalSlamFrames/2) > iSimFrame )
    {
        rTuple.pView->move(sf::Vector2f(vSlamForce.x*-1.0,vSlamForce.y*1.0));
    }
    else if (m_iLastShakeFrame + iTotalSlamFrames <= iSimFrame)
    {
        rTuple.pView->setCenter(sf::Vector2f(rTuple.fScreenWidth/2,rTuple.fScreenHeight/2));
    }

    rTuple.pRenderWindow->setView(*rTuple.pView);
}

//------------------------------------------------------------

void GameScreen::CreateMiddleLine(const DataStruct& rTuple)
{
    static const float lineHeight = (rTuple.fScreenHeight/m_iNumberOfLines)/2;
    static const float lineThickness = 7.5f;

    for (int i = m_iNumberOfLines; i >= 0; i--)
    {
        m_DashedLineRect[i].setSize(sf::Vector2f(lineThickness,lineHeight));
        m_DashedLineRect[i].setOrigin(sf::Vector2f(m_DashedLineRect[i].getSize().x/2,0.0f));
        m_DashedLineRect[i].setFillColor(sf::Color( 255,255,225,100));
        m_DashedLineRect[i].setPosition(sf::Vector2f(rTuple.fScreenWidth/2+7.5f, i*2 *lineHeight - lineHeight*0.5 ));
 
    }
}

//------------------------------------------------------------

void GameScreen::DimMiddleLine(const DataStruct& rTuple, const bool bShouldDimLine)
{
    static const int iDimValue = 45;
    static const int iFullOpacityValue = 100;
    for (int i = m_iNumberOfLines; i >= 0; i--)
    {
        //todo make this gradual so that its not so jarring
        if (bShouldDimLine)
        {
            m_DashedLineRect[i].setFillColor(sf::Color( 255,255,225,iDimValue));
        }
        else if(!bShouldDimLine)
        {
            float iNewAlpha = m_DashedLineRect[i].getFillColor().a +1 ;
            if (iNewAlpha <= iFullOpacityValue )
            {
                m_DashedLineRect[i].setFillColor(sf::Color( 255,255,225,iNewAlpha));
            }
            else
            {
                m_DashedLineRect[i].setFillColor(sf::Color( 255,255,225,iFullOpacityValue));
            }

        }
    }
}

//------------------------------------------------------------

void GameScreen::SetBoundryEdgeShapes(const DataStruct& rTuple)
{

    //TODO: move this and the other vfx out of the GameScreen System

    const bool bLastHitTop = rTuple.pWorldState->GetDidBallLastHitScreenTop();

    sf::RectangleShape* pShape = bLastHitTop ? m_sTopEdge : m_sBottomEdge;

    for(int i = m_iBounceVFXArrayLength; i >= 0; i --)
    {
        sf::Color EdgeColor(120,120,255,200);
        pShape[i].setSize(sf::Vector2f (50.0f, 30.0f));
        pShape[i].setFillColor(EdgeColor);

        if(bLastHitTop)
        {
            pShape[i].setOrigin(pShape[0].getSize().x/2,pShape[0].getSize().y);
            pShape[i].setPosition(sf::Vector2f(rTuple.pBall->GetShape().getPosition().x, 0.0f));
            return;
        }
        else
        {
            pShape[i].setOrigin(pShape[0].getSize().x/2, 0.0f);
            pShape[i].setPosition(sf::Vector2f(rTuple.pBall->GetShape().getPosition().x, rTuple.fScreenHeight));
            return;
        }
    }
    
}

//------------------------------------------------------------

void GameScreen::UpdateWallBounceVFX(const DataStruct& rTuple)
{

    static constexpr u_int8_t iFXFrameTime = 20;

    //todo change color to more of a red the closer to the edge of the paddle that the ball is hit
    //i will later tie this into ball controll so hits near the edge have more spread and the middle is the sweet spot

    const bool bLastHitTop = rTuple.pWorldState->GetDidBallLastHitScreenTop();

    sf::RectangleShape* pFXShape = bLastHitTop ? m_sTopEdge : m_sBottomEdge;

    const float fSpeed = rTuple.pBall->GetYSpeed();
    for(int8_t i = GameScreen::m_iBounceVFXArrayLength; i >= 0 ; i--)
    {
        if (GetSimFrameTopLastHit() > 0 && GetSimFrameTopLastHit() + iFXFrameTime > rTuple.pWorldState->GetCurrentSimFrame())
        {
            if(GetSimFrameTopLastHit() == rTuple.pWorldState->GetCurrentSimFrame())
            {
                pFXShape[i].setSize(sf::Vector2f (abs(fSpeed)/10, 20.0f));
            }
            pFXShape[i].setSize(sf::Vector2f (abs(fSpeed)/(10 - i), 20.0f));
            pFXShape[i].setOrigin(pFXShape->getGlobalBounds().width/2, pFXShape->getGlobalBounds().height/2);
        }
    else
    {
        const sf::Color pLastColor = pFXShape[0].getFillColor();
        const sf::Color fillColor = sf::Color(pLastColor.r,pLastColor.g,pLastColor.b,pLastColor.a-i);
        pFXShape[i].setSize(sf::Vector2f (pFXShape->getSize().x /2 ,pFXShape->getSize().y /i));
        pFXShape[i].setOrigin(pFXShape->getGlobalBounds().width/2, pFXShape->getGlobalBounds().height/2);
        pFXShape[i].setFillColor(fillColor);
    }   

    }
}


//------------------------------------------------------------

bool GameScreen::ShouldAttachBallToBat(const DataStruct& rTuple)
{
    if ( rTuple.pBall->GetCurrentBallState() == eBallState::AtPlayer1 )
    {
        return true;
    }
    else if ( rTuple.pBall->GetCurrentBallState() == eBallState::AtPlayer2 )
    {
        return true;
    }
    return false;
}