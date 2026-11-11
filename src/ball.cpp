#include <headers/ball.h>
#include <assert.h>

static constexpr float g_BALLSIZE = 15.0f;
Ball::Ball(std::shared_ptr<sf::RenderWindow> pRenderWindow)
{
    
    m_vBallVector = sf::Vector3f( pRenderWindow->getSize().x/2, pRenderWindow->getSize().y/2, 0 );
    m_sShape.setSize(sf::Vector2f(g_BALLSIZE,g_BALLSIZE));
    m_sShape.setFillColor(sf::Color::White);
    m_sShape.setOrigin(sf::Vector2f(m_sShape.getSize().x/2,m_sShape.getSize().y/2));
    m_sShape.setPosition(m_vBallVector.x ,m_vBallVector.y);
}

//----------------------------------------------------------

void Ball::StateMachine()
{
    const eBallState eCurrentBallState = GetCurrentBallState();
    const eBallState eDesiredBallState = GetDesiredBallState();
    eBallState eNewBallState = eCurrentBallState;

    if (eCurrentBallState == eBallState::ResetGamePosition )
    {
        m_sShape.setPosition(m_vBallVector.x ,m_vBallVector.y);
        //game reset state will go to either left or right
        if (eDesiredBallState == eBallState::AtPlayer1)
        {
            eNewBallState = eBallState::AtPlayer1;
        }
        else if (eDesiredBallState == eBallState::AtPlayer2)
        {
            eNewBallState = eBallState::AtPlayer2;
        }
        else
        {
            eNewBallState = eBallState::LEFT;
        }
    }
    else if (eCurrentBallState == eBallState::GoalOnPlayer1 )
    {
        eNewBallState = eBallState::AtPlayer1;
    }
        else if (eCurrentBallState == eBallState::GoalOnPlayer2 )
    {
        eNewBallState = eBallState::AtPlayer2;
    }
    else if (eCurrentBallState == eBallState::AtPlayer1 )
    {
        if( eDesiredBallState == eBallState::RIGHT) 
        {
            
            m_v2CurrentBallSpeed.x = abs(m_v2InitialSpeed.x)*-1.0f;
            eNewBallState = eDesiredBallState;
        }
    }
    else if (eCurrentBallState == eBallState::AtPlayer2)
    {
        if (eDesiredBallState == eBallState::LEFT)
        {
            m_v2CurrentBallSpeed.x = abs(m_v2InitialSpeed.x);
            eNewBallState = eDesiredBallState;
        }
    }
    else if(eCurrentBallState == eBallState::LEFT || eCurrentBallState == eBallState::RIGHT )
    {
        //check if our collision valid
        const bool bIsCollisionValid = eCurrentBallState == eBallState::LEFT && GetLastCollisionType() == eCollisionType::CollisionWithPlayer2 ||
        eCurrentBallState == eBallState::RIGHT && GetLastCollisionType() == eCollisionType::CollisionWithPlayer1 ;

        if ( eDesiredBallState == eBallState::HitBall && bIsCollisionValid )
        {
            //inverse the desired move state
            eNewBallState = eCurrentBallState == eBallState::LEFT ? eBallState::RIGHT : eBallState::LEFT;
            m_v2CurrentBallSpeed.x *= -1.0f;
            if (abs(m_v2CurrentBallSpeed.x) < m_fTopSpeed )
            {
                float fSpeedUpBoost = IsLastHitOnSweetSpot() ? m_fSpeedUpIncriment * 1.1f : m_fSpeedUpIncriment * 0.85f ;
                m_v2CurrentBallSpeed.x = m_v2CurrentBallSpeed.x > 0.0f ? m_v2CurrentBallSpeed.x += fSpeedUpBoost : m_v2CurrentBallSpeed.x -= fSpeedUpBoost;
            }
        }
        else if( eDesiredBallState == eBallState::GoalOnPlayer1)
        {
            eNewBallState = eBallState::AtPlayer2;
        }
        else if ( eDesiredBallState == eBallState::GoalOnPlayer2 )
        {
            eNewBallState = eBallState::AtPlayer1;
        }
            else if(eDesiredBallState == eBallState::HitBottomWall || eDesiredBallState == eBallState::HitTopWall )
        {
            if (m_v2CurrentBallSpeed.y <= m_fTopSpeed)
            {
                SetYSpeed(GetYSpeed()*-1.1f);
            }
            else
            {
                SetYSpeed(GetYSpeed()*-1.0f);
            }
            eNewBallState = m_eCurrentBallState;
        }
    }

    assert(eNewBallState != eBallState::None && "StateMachine is NONE Update When Called to after scoring a goal");
    SetCurrentBallState(eNewBallState);
}

//----------------------------------------------------------

void Ball::UpdateBallPosition(const float fDeltaT,  const bool isPaused)
{
    if (isPaused)
    { 
        return; 
    }
    m_vBallVector.x -= GetXSpeed()*fDeltaT;
    m_vBallVector.y -= GetYSpeed()*fDeltaT;
    m_sShape.setPosition(GetTranslationPosition().x, GetTranslationPosition().y);
}

//----------------------------------------------------------

void Ball::OnBatCollision(const float fScreenWidth)
{

    SetDesiredBallState(eBallState::HitBall);
    StateMachine();

    if(GetCurrentBallState() == eBallState::LEFT)
    {
        SetLastCollisiontype( eCollisionType::CollisionWithPlayer2);
    }
    else
    {
         SetLastCollisiontype( eCollisionType::CollisionWithPlayer1);
    }
}

//----------------------------------------------------------

void Ball::OnWallCollision(const bool isColliding, const float fScreenHeight)
{
    if (isColliding)
    {
        if( m_vBallVector.y < fScreenHeight/2.0f)
        {
            SetDesiredBallState(eBallState::HitTopWall);
        }
        else
        {
            SetDesiredBallState(eBallState::HitBottomWall);
        }
        StateMachine();
        SetLastCollisiontype( eCollisionType::CollisionWithWall);

    }
}

//----------------------------------------------------------

void Ball::OnScoreGoal(const bool isLeft)
{
        if (isLeft)
        {
            SetDesiredBallState(eBallState::GoalOnPlayer1);
        }
        else
        {
            SetDesiredBallState(eBallState::GoalOnPlayer2);
        }
        StateMachine();
        SetLastCollisiontype( eCollisionType::CollisionWithGoalZone);
        
}

//----------------------------------------------------------

void Ball::UpdateBallTrail(const int64 iSimFrame)
{
    if (iSimFrame % 3 == 0)
    {
        for (int8 i = GetTrailShapeArrayLength(); i >= 0; i --)
        {
            m_sShapeTrail[0] = m_sShape;
            m_sShapeTrail[i] = m_sShapeTrail[i-1];
            const float fScaleModifier = GetTrailShapeArrayLength() - 0.08f;
            
            const bool bIsGoingRight = GetCurrentBallState() == eBallState::RIGHT;

            const sf::Color TrailColor = bIsGoingRight ? sf::Color(255,100,100,230/i) : sf::Color(100,100,255,240/i*2);

            m_sShapeTrail[i].setFillColor(TrailColor);
            m_sShapeTrail[i].setScale(sf::Vector2f(fScaleModifier/GetTrailShapeArrayLength(),fScaleModifier/GetTrailShapeArrayLength()));

        }
    }
}

//----------------------------------------------------------
