#include <headers/batSystem.h>

BatSystem::BatSystem()
{
    //does nothing for now
}

eBatMoveDirection BatSystem::DetermCurrentMoveDirection(const DataStruct& rTuple, std::shared_ptr<Bat> pBat)
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
    pBat->SetCurrentMoveDirection(eNewMoveDirection); // should i be setting this here if its returning the value?
    return eNewMoveDirection;
}



//-----------------------------------------------------------------

void BatSystem::CalculateBatSpeed(const DataStruct& rTuple,std::shared_ptr<Bat> pBat, const float fDeltaT)
{
    eBatMoveDirection newMoveDirection = DetermCurrentMoveDirection(rTuple,pBat);
    const float fSpeed =  (pBat->GetVelocity()*fDeltaT);
    if (rTuple.pWorldState->GetCurrentGameState() != eGameState::Paused)
    {
        if ( abs(fSpeed) < pBat->GetTopSpeed())
        {
            pBat->SetPosition(sf::Vector2f(pBat->GetPosition().x, pBat->GetPosition().y + fSpeed ));
        }

        if (newMoveDirection == eBatMoveDirection::UP && abs(fSpeed) < pBat->GetTopSpeed())
        {
            pBat->ModifyVelocity( - ( pBat->GetAccel()*fDeltaT )* abs(pBat->GetAnalogSpeedModifier()/100.0f));
        }
        else if (newMoveDirection == eBatMoveDirection::DOWN && abs(fSpeed) < pBat->GetTopSpeed())
        { 
            pBat->ModifyVelocity(( pBat->GetAccel()*fDeltaT )* pBat->GetAnalogSpeedModifier()/100.0f);
        }
        else
        {
            pBat->SetVelocity(fSpeed*0.96/fDeltaT);
        }

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
