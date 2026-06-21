#include <headers/gameScreen.h>
#include <headers/batSystem.h>
#include <headers/gameStateSystem.h>
#include <headers/gamePadUtils.h>
#include <headers/collisionSystem.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cassert>


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
    SetScoreText(rTuple.pWorldState->GetScore()[0],rTuple.pWorldState->GetScore()[1]);

    //SetBoundryEdgeShapes(rTuple);
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

static uint16 CalculateFrameRate(const DataStruct& rTuple)
{
    if(!rTuple.pRenderWindow->hasFocus())
    {
        return 10;
    }
    else if ( rTuple.pWorldState->GetCurrentGameState() != eGameState::Running )
    {
        return 60;
    }
    else
    {
        return 240;
    }
}

//-----------------------------------------------------------------

int GameScreen::UpdateGamescreen(const DataStruct& rTuple, sf::Clock &rGameClock)
{
    rTuple.pGameMusic->pause();
    rTuple.pGameMusic->setLoop(true);
    rTuple.pBall->SetYSpeed(200);


    rTuple.pRenderWindow->setMouseCursorVisible(false);

    while (rTuple.pRenderWindow->isOpen())
    {

        const int64 iSimFrame = rTuple.pWorldState->GetCurrentSimFrame();
        const float fDeltaT = rGameClock.restart().asSeconds();
        const float fFps = 1.0 / fDeltaT;
        //std::cout << fFps << " Frames Per Second: \n";
        
        rTuple.pWorldState->DetermineGameState();
        rTuple.pRenderWindow->setFramerateLimit(CalculateFrameRate(rTuple));
        
        std::unique_ptr<GameStateSystem>pGameStateSystem = std::make_unique<GameStateSystem>();

        const bool bIsRunning = rTuple.pWorldState->GetCurrentGameState() == eGameState::Running;
        if (rTuple.pWorldState->GetCurrentGameState() == eGameState::Boot )
        {
            pGameStateSystem->ResetGame(rTuple);
        }
        
        std::unique_ptr<BatSystem> pBatSystem = std::make_unique<BatSystem>();
        std::unique_ptr<PadUtils> pPadUtils = std::make_unique<PadUtils>();
        std::unique_ptr<CollisionSystem> pCollisionSystem = std::make_unique<CollisionSystem>();

        std::vector<std::shared_ptr<Bat>> pBatArray;
        pBatArray.push_back(rTuple.pBat1);
        pBatArray.push_back(rTuple.pBat2);

        for(size_t i = 0 ; i< pBatArray.size() ; i++)
        {
            eBatMoveDirection BatMoveDirection = pBatSystem->CaclulateCurrentMoveDirection(rTuple,pBatArray[i]);
            pBatSystem->CalculateBatSpeed(rTuple,pBatArray[i], fDeltaT);
            pBatArray[i]->SetCurrentMoveDirection(BatMoveDirection);
        }

        rTuple.pBall->UpdateBallPosition(fDeltaT/10, !bIsRunning); //TODO: make this feel good in the actual values with dividing by 10
        
        const eCollisionType eCollidingWith = pCollisionSystem->CheckCollisions(rTuple);
        if ( eCollidingWith != eCollisionType::NoCollision )
        {
             pCollisionSystem->HandleCollisions(rTuple, eCollidingWith, iSimFrame);
        }

        SetLastCollisionType(eCollidingWith);

        ShakeScreen(rTuple,0.25f, eCollidingWith);

        std::shared_ptr<Bat> pBat =  pBatSystem->WhichBatShouldBallAttatchTo(rTuple);
        if(pBat != nullptr)
        {
            pBatSystem->AttachBallToBat(rTuple,pBat);
        }

        const bool bUpdatedUiText = UpdateUIText( rTuple.pWorldState->GetisWinConditionMet(), !bIsRunning, rTuple );

        DimMiddleLine(rTuple, bUpdatedUiText || rTuple.pWorldState->IsScreenShaking());
        UpdateScoreText(rTuple, iSimFrame, !bIsRunning);

        if(rTuple.pBat1->GetLastHitFrame() > rTuple.pBat2->GetLastHitFrame())
        {
            rTuple.pBat1->UpdateHitVFX(iSimFrame,rTuple.pBall->IsLastHitOnSweetSpot());
        }
        else
        {
            rTuple.pBat2->UpdateHitVFX(iSimFrame,rTuple.pBall->IsLastHitOnSweetSpot());  
        }

        HandleEvents(rTuple);
        RenderElements(rTuple);
        
        rTuple.pRenderWindow->display();
        if(bIsRunning)
        {
            rTuple.pBall->UpdateBallTrail( iSimFrame );
            //UpdateWallBounceVFX(rTuple);
            rTuple.pWorldState->IncrimentSimFrame();
        }
    }

    return 1;
}


//----------------------------------------------------------

std::string GameScreen::SetScoreText(const unsigned short &iPlayer1Score, const unsigned short &iPlayer2Score)
{
   return std::to_string(iPlayer1Score)+"-"+ std::to_string(iPlayer2Score);
}

//----------------------------------------------------------

void GameScreen::UpdateScoreText(const DataStruct& rTuple, const unsigned long iSimFrame, const bool bIsPaused)
{
    if (bIsPaused)
    {
        return;
    }
    const std::string scoreString = GameScreen::SetScoreText(rTuple.pWorldState->GetScore()[0],rTuple.pWorldState->GetScore()[1]);
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

//-----------------------------------------------------------

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


void GameScreen::ShakeScreen(const DataStruct &rTuple, const float fMagnitude, const eCollisionType eJustHit )
{
    const int iSimFrame = rTuple.pWorldState->GetCurrentSimFrame();
    static constexpr int iTotalSlamFrames = 15;
    static const sf::Vector2 vForceScalingRatio(10,5);
    static constexpr float fForceScale = 600;
    static const sf::Vector2f vScaledForces(vForceScalingRatio.x*fForceScale,vForceScalingRatio.y*fForceScale);

    if (rTuple.pWorldState->GetCurrentGameState() != eGameState::Running)
    {
        return;
    }

    const sf::Vector2f vSlamForce = sf::Vector2f(fMagnitude*rTuple.pBall->GetXSpeed()/vScaledForces.x,fMagnitude*rTuple.pBall->GetYSpeed()/vScaledForces.y);

    if (eJustHit == eCollisionType::CollisionWithWall)
    {
        rTuple.pWorldState->SetSimFrameTopLastHit(iSimFrame);
        m_iLastShakeFrame = iSimFrame;
    }
    else if ( eJustHit == eCollisionType::CollisionWithPlayer1 || eJustHit == eCollisionType::CollisionWithPlayer2 )
    {
        m_iLastShakeFrame = iSimFrame;
    }

    if( m_iLastShakeFrame + (iTotalSlamFrames) > iSimFrame && m_iLastShakeFrame > 0)
    {
        rTuple.pView->move(sf::Vector2f(vSlamForce.x*-1.0,vSlamForce.y*1.0));
        rTuple.pWorldState->SetIsScreenShaking(true);
    }
    else if (m_iLastShakeFrame + iTotalSlamFrames <= iSimFrame)
    {
        rTuple.pView->setCenter(sf::Vector2f(rTuple.fScreenWidth/2,rTuple.fScreenHeight/2));
        rTuple.pWorldState->SetIsScreenShaking(false);
    }

    rTuple.pRenderWindow->setView(*rTuple.pView);
}

//------------------------------------------------------------

void GameScreen::CreateMiddleLine(const DataStruct& rTuple)
{
    static const float lineHeight = (rTuple.fScreenHeight/m_iNumberOfLines)/2;
    static const float lineThickness = 7.5f;

    for (short i = m_iNumberOfLines; i >= 0; i--)
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
    static const unsigned short iDimValue = 45;
    static const unsigned int iFullOpacityValue = 100;
    for (short i = m_iNumberOfLines; i >= 0; i--)
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

    for(short i = m_iBounceVFXArrayLength; i >= 0; i --)
    {
        sf::Color EdgeColor(120,120,255,50+(i*5));
        pShape[i].setSize(sf::Vector2f (40.0f*i, 30.0f));
        pShape[i].setFillColor(EdgeColor);

        if(bLastHitTop)
        {
            pShape[i].setOrigin(pShape[0].getSize().x/2,pShape[0].getSize().y);
            pShape[i].setPosition(sf::Vector2f(rTuple.pBall->GetShape().getPosition().x, -30.0f));
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

    constexpr unsigned short iFXFrameTime = 20;
    const int64 iSimFrame = rTuple.pWorldState->GetCurrentSimFrame();

    const bool bLastHitTop = rTuple.pWorldState->GetDidBallLastHitScreenTop();

    sf::RectangleShape* pFXShape = bLastHitTop ? m_sTopEdge : m_sBottomEdge;

    const float fSpeed = rTuple.pBall->GetYSpeed();
    for(short i = GameScreen::m_iBounceVFXArrayLength; i >= 0 ; i--)
    {
        if (rTuple.pWorldState->GetSimFrameTopLastHit() > 0 && rTuple.pWorldState->GetSimFrameTopLastHit() + iFXFrameTime > rTuple.pWorldState->GetCurrentSimFrame())
        {
            if(rTuple.pWorldState->GetSimFrameTopLastHit() == iSimFrame)
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

void GameScreen::HandleEvents(const DataStruct& rTuple)
{
    sf::Event event;
    while (rTuple.pRenderWindow->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            rTuple.pRenderWindow->close();
        }
        Command* command = m_hInputHandler.HandleInput( &event, rTuple.pWorldState->GetisWinConditionMet());
        if (command) 
        {  
            command->execute(rTuple);
        }
    }
}

//------------------------------------------------------------

void GameScreen::RenderElements(const DataStruct& rTuple)
{
rTuple.pRenderWindow->clear();

    for(int8 i = m_iNumberOfLines; i>=0 ; i--)
    {
        rTuple.pRenderWindow->draw( m_DashedLineRect[i] );
    }

    rTuple.pRenderWindow->draw( *rTuple.pMessageText );
    rTuple.pRenderWindow->draw( *rTuple.pScoreText );

    if (rTuple.pBall->GetCurrentBallState() != eBallState::AtPlayer1 && 
    rTuple.pBall->GetCurrentBallState() != eBallState::AtPlayer2 )
    {
        for (short i = rTuple.pBall->GetTrailShapeArrayLength(); i >= 0; i --)
        {
            rTuple.pRenderWindow->draw( rTuple.pBall->GetTrailShapeArray()[i] );
        }
    }
    
    //TODO: Get the vfx for the edge bounce working better
    
    for(int8 i = m_iBounceVFXArrayLength; i >= 0; i --)
    {
        if (rTuple.pBall->GetDesiredBallState() == eBallState::HitBottomWall || rTuple.pBall->GetDesiredBallState() == eBallState::HitTopWall) 
        {
            SetBoundryEdgeShapes(rTuple);
        }

        if (rTuple.pWorldState->GetSimFrameTopLastHit() + 30 > rTuple.pWorldState->GetCurrentSimFrame())
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

    }

    for (int8 i = rTuple.pBat1->GetBatVFXArrayLength(); i >= 0; i --)
    {
        rTuple.pRenderWindow->draw( rTuple.pBat1->GetBatVFXShapeArray()[i]);
        rTuple.pRenderWindow->draw( rTuple.pBat2->GetBatVFXShapeArray()[i]);
    }

    rTuple.pRenderWindow->draw( rTuple.pBat1->GetShape());
    rTuple.pRenderWindow->draw( rTuple.pBat2->GetShape());

    rTuple.pRenderWindow->draw( rTuple.pBall->ReferenceShape() );
}