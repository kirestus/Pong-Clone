
#include <headers/debugText.h>

sf::String DebugText::DebugTextGameState( eGameState eGameState )
{
    switch(eGameState)
    {
        case eGameState::Paused:
            return sf::String("Paused:");
            break;
        
        case eGameState::Boot:
            return sf::String("Boot:");
            break;
        
        case eGameState::Running:
            return sf::String("Running:");
            break;
        
        case eGameState::GameOver:
            return sf::String("GameOver:");
            break;

        default:
            return sf::String("");
    }

}
//----------------------------------------------------------

sf::String DebugText::DebugTextBallState( eBallState eDebugBallState )
{
    switch(eDebugBallState)
    {
        case  eBallState::LEFT:
            return sf::String("LEFT:");
            break;

        case eBallState::RIGHT:
            return sf::String("RIGHT:");
            break;

        case eBallState::HitBall:
            return sf::String("HitBall:");
            break; 
        
        case eBallState::HitTopWall:
        case eBallState::HitBottomWall:
            return sf::String("HitWall:");
            break;

        case eBallState::ResetGamePosition:
            return sf::String("RESET:");
            break;

        case eBallState::AtPlayer1:
            return sf::String("AT P1:");
            break;

        case eBallState::AtPlayer2:
            return sf::String("AT P2:");
            break;

        default:
            return sf::String("Undefined BallState");
            break;

    }
}


