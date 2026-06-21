#include <headers/collisionSystem.h>
#include<headers/gameStateSystem.h>
#include <cassert>

//static global variables 
static constexpr float g_fSWEET_SPOT_SIZE = 20.0f;


float CollisionSystem::CreateRandomAngle(const int16 maxRange)
{
    return rand() % (maxRange);
}
//----------------------------------------------------------

bool CollisionSystem::isBallCollidingWithTarget(const sf::FloatRect box1, const sf::FloatRect box2)
{
    return box1.intersects(box2);
}

//----------------------------------------------------------

bool CollisionSystem::isBallHittingWall(const sf::FloatRect box1, const std::shared_ptr<sf::RenderWindow> pRenderWindow)
{
    const float rectTop = 0.0f;
    const float rectBottom = pRenderWindow->getSize().y - 15.0f;
    return box1.top >= rectBottom || box1.top <= rectTop;
}

//----------------------------------------------------------

bool CollisionSystem::isBallHittingGoal(const sf::FloatRect box1,const DataStruct &rTuple/*,const bool isPaused*/ )
{
    const float fLeftGoalPosition = 0.0f;
    const float fRightGoalPosition = rTuple.fScreenWidth;
    const float fBoxXpos = box1.left;

    if (fBoxXpos < fLeftGoalPosition )
    {
        //m_aScore[1] ++;

        return true;
    }
    else if (fBoxXpos + 15 > fRightGoalPosition)
    {   
        //m_aScore[0] ++;
        
        return true;
    }
    return false;
}

//----------------------------------------------------------

eCollisionType CollisionSystem::CheckCollisions(const DataStruct &rTuple)
{  
    const sf::FloatRect pBallHitbox = rTuple.pBall->GetShape().getGlobalBounds();
    
    //TODO: see if i cant write this to be faster, seems slow to do all the bool checks and then check the if statements but it's easy to read
    const bool bIsCollidingWithP1 = isBallCollidingWithTarget(pBallHitbox, rTuple.pBat1->GetShape().getGlobalBounds());
    const bool bIsCollidingWithP2 = isBallCollidingWithTarget(pBallHitbox, rTuple.pBat2->GetShape().getGlobalBounds());
    const bool bIsCollidingWithWalls = isBallHittingWall(pBallHitbox, rTuple.pRenderWindow );
    const bool bDidPLayerScore = isBallHittingGoal(pBallHitbox,rTuple );

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

void CollisionSystem::HandleCollisions(const DataStruct &rTuple, const eCollisionType eCollidingwith,const int64 iSimFrame )
{

    switch (eCollidingwith)
    {
    case eCollisionType::CollisionWithGoalZone:
    {
        const bool bIsleft = rTuple.pBall->GetTranslationPosition().x < rTuple.fScreenWidth/2;
        rTuple.pBall->OnScoreGoal(bIsleft, rTuple.fScreenWidth);
        rTuple.pWorldState->SetSimFrameOfLastScoredGoal(iSimFrame);
        if ( rTuple.pWorldState->GetisWinConditionMet() )
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

        std::unique_ptr<GameStateSystem>pGameStateSystem = std::make_unique<GameStateSystem>();
        pGameStateSystem->ResetGame(rTuple);

        return;
    }
    case eCollisionType::CollisionWithPlayer1:
    case eCollisionType::CollisionWithPlayer2:
    {
        if(rTuple.pBall->GetLastCollisionType() == eCollidingwith)
        {
            //ignore multiple collisions with a single paddle
            return;
        }
        const bool bIsCollidingWithP1 = eCollidingwith == eCollisionType::CollisionWithPlayer1 ? true : false; 
        const std::shared_ptr<Bat> pBat = bIsCollidingWithP1 ? rTuple.pBat1 : rTuple.pBat2 ;
        const eBallState eBallGoingDir = bIsCollidingWithP1 ? eBallState::RIGHT : eBallState::LEFT;
        const sf::Vector3f ballPosition(rTuple.pBall->GetTranslationPosition().x,rTuple.pBall->GetTranslationPosition().y,100.0f);
        
        rTuple.pBall->OnBatCollision(rTuple.fScreenHeight);

        const float fCollisionDelta = abs(rTuple.pBall->GetShape().getPosition().y - pBat->GetShape().getPosition().y) ;
        const bool bIsHittingSweetSpot = fCollisionDelta < g_fSWEET_SPOT_SIZE; //TODO: make this a global variable somewhere
        rTuple.pBall->SetLastHitOnSweetSpot(bIsHittingSweetSpot);

        constexpr int16 iRandomSpread(500); // add a bit of randomness to each hit
        
        const float fNewYSpeed = bIsHittingSweetSpot ? pBat->GetVelocity()*1.25f + rTuple.pBall->GetYSpeed() : pBat->GetVelocity() + rTuple.pBall->GetYSpeed()+CreateRandomAngle(iRandomSpread) ; 
        rTuple.pBall->SetYSpeed(fNewYSpeed); 
        rTuple.pBall->SetDesiredBallState(eBallGoingDir);
        rTuple.pBall->StateMachine(rTuple.fScreenWidth);
        pBat->SetLastHitFrame(iSimFrame);
        
        sf::Sound* pPlayThisSound = eBallGoingDir != eBallState::LEFT ? rTuple.pPlayer1SoundEffect :rTuple.pPlayer2SoundEffect;
        pPlayThisSound->setPosition(ballPosition);
        if (rTuple.pWorldState->GetShouldPlaySFX())
        {
            pPlayThisSound->play();
        }
        return;
    }
    case eCollisionType::CollisionWithWall:
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
        return;
    }
    default:
    assert("Undefined Collision");
}
}


