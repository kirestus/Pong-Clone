
#include <headers/debugText.h>

sf::String DebugText::DebugTextGameState( eGameState eGameState )
{
    switch(eGameState)
    {
        case eGameState::Paused:
            return sf::String("Paused:");
        
        case eGameState::Boot:
            return sf::String("Boot:");
        
        case eGameState::Running:
            return sf::String("Running:");
        
        case eGameState::GameOver:
            return sf::String("GameOver:");

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

        case eBallState::RIGHT:
            return sf::String("RIGHT:");

        case eBallState::HitBall:
            return sf::String("HitBall:");
        
        case eBallState::HitTopWall:
        case eBallState::HitBottomWall:
            return sf::String("HitWall:");

        case eBallState::ResetGamePosition:
            return sf::String("RESET:");

        case eBallState::AtPlayer1:
            return sf::String("AT P1:");

        case eBallState::AtPlayer2:
            return sf::String("AT P2:");

        default:
            return sf::String("Undefined BallState");

    }
}


