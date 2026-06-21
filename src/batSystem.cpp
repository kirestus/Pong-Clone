#include <headers/batSystem.h>
#include<iostream>

BatSystem::BatSystem()
{
    //does nothing for now
}

//-----------------------------------------------------------------

eBatMoveDirection BatSystem::CaclulateCurrentMoveDirection(const DataStruct& rTuple, std::shared_ptr<Bat> pBat)
{
    const eBatMoveDirection eCurrentMoveDirection = pBat->GetCurrentMoveDirection();
    const eBatMoveDirection eDesiredMoveDirection = pBat->GetDesiredMoveDirection();
    eBatMoveDirection eNewMoveDirection;

    switch(eCurrentMoveDirection)
    {
        case eBatMoveDirection::NONE :
            eNewMoveDirection = eDesiredMoveDirection;
            break;
        case eBatMoveDirection::DOWN :
        {
            eNewMoveDirection = pBat->IsHittingBottom(rTuple.pRenderWindow->getSize() ) ? eBatMoveDirection::UP : eNewMoveDirection = eDesiredMoveDirection;
            break;
        }
        case eBatMoveDirection::UP :
        {
            eNewMoveDirection = pBat->IsHittingTop() ? eBatMoveDirection::NONE : eNewMoveDirection = eDesiredMoveDirection;
            break;
        }
        default:
            break;   
    }
    return eNewMoveDirection;
}

//-----------------------------------------------------------------

void BatSystem::CalculateBatSpeed(const DataStruct& rTuple,std::shared_ptr<Bat> pBat, const float fDeltaT)
{
    eBatMoveDirection eNewMoveDirection = pBat->GetCurrentMoveDirection();
    const float fDistance =  (pBat->GetVelocity() * fDeltaT);
    const float fDownSpeed = (pBat->GetAccel()+fDistance) * fDeltaT;
    const float fUpSpeed = (-pBat->GetAccel()+fDistance) * fDeltaT;

    if (rTuple.pWorldState->GetCurrentGameState() == eGameState::Running)
    {
        pBat->SetPosition(sf::Vector2f(pBat->GetPosition().x, pBat->GetPosition().y + fDistance ));

        if (eNewMoveDirection == eBatMoveDirection::UP && abs(fUpSpeed) < pBat->GetTopSpeed())
        {
            std::cout<<"UPSpeed"<<abs(fUpSpeed)<<"\n";
            pBat->ModifyVelocity( fUpSpeed * abs(pBat->GetAnalogSpeedModifier()/100.0f));
        }
        else if (eNewMoveDirection == eBatMoveDirection::DOWN && abs(fDownSpeed) < pBat->GetTopSpeed())
        { 
            std::cout<<"DownSpeed"<<abs(fDownSpeed)<<"\n";
            pBat->ModifyVelocity( fDownSpeed * abs(pBat->GetAnalogSpeedModifier()/100.0f));
        }

        pBat->SetVelocity(pBat->GetVelocity() * pBat->GetDecayRate());
        

        if( ( pBat->IsHittingBottom(rTuple.pRenderWindow->getSize()) || pBat->IsHittingTop() ))
        {
            NudgeBat(rTuple, pBat);
            pBat->SetVelocity(0.0f);
        }
        
        pBat->UpdateShapeToDesiredTransform();
    }

}

//-----------------------------------------------------------------


void BatSystem::NudgeBat(const DataStruct& rTuple , std::shared_ptr<Bat> pBat)
{
    //Nudge the bat if its at the top or bottom so it doesnt glitch out
    if( pBat->IsHittingTop() )
    {
        sf::Vector2f nudgeDown = sf::Vector2f(pBat->GetPosition().x,pBat->GetPosition().y+0.1);
        pBat->SetPosition(nudgeDown);
    }
    else if( pBat->IsHittingBottom( rTuple.pRenderWindow->getSize() ) )
    {
        sf::Vector2f nudgeUp = sf::Vector2f(pBat->GetPosition().x,pBat->GetPosition().y-0.1);
        pBat->SetPosition(nudgeUp);
    }
}
//-----------------------------------------------------------------

std::shared_ptr<Bat> BatSystem::WhichBatShouldBallAttatchTo(const DataStruct& rTuple)
{
    if ( rTuple.pBall->GetCurrentBallState() == eBallState::AtPlayer1)
    {
        return rTuple.pBat1;
    }
    else if (rTuple.pBall->GetCurrentBallState() == eBallState::AtPlayer2)
    {
        return rTuple.pBat2;
    }

    return nullptr;
}

//-----------------------------------------------------------------

void BatSystem::HandleBallAttachements(const DataStruct& rTuple)//you spicy boy ;)
{
    if ( rTuple.pBall->GetCurrentBallState() == eBallState::AtPlayer1 )
    {
        AttachBallToBat(rTuple, rTuple.pBat1);
    }
    else
    {
        AttachBallToBat(rTuple, rTuple.pBat2);
    }

}

//-----------------------------------------------------------------

void BatSystem::AttachBallToBat(const DataStruct& rTuple, std::shared_ptr<Bat> pBat)
{
    
    rTuple.pBall->StateMachine(rTuple.fScreenWidth);
    rTuple.pBall->SetYSpeed(pBat->GetVelocity()*-1.9f);

    const sf::Vector2f vBatPosition = pBat->GetShape().getPosition();
    const float fOffset = pBat->GetPlayerNumber() == ePlayerNumber::PLAYER1  ? 20.0f : -20.0f;
    rTuple.pBall->SetBallVector(sf::Vector3f( vBatPosition.x + fOffset, vBatPosition.y, 0.0f));
}
