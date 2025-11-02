#include <SFML/Graphics.hpp>
#include <headers/dataStruct.h>
#include <string>

class DebugText
{
    public:
    static sf::String DebugTextGameState( eGameState eGameState )
    {
        if (eGameState == eGameState::Paused)
        {
            return sf::String("Paused:");
        }
        else if (eGameState == eGameState::Boot)
        {
            return sf::String("Boot:");
        }
        else if (eGameState == eGameState::Running)
        {
            return sf::String("Running:");
        }
        else if (eGameState == eGameState::GameOver)
        {
            return sf::String("GameOver:");
        }
        else
        {
            return sf::String("");
        }

        return sf::String("");
    }
    //----------------------------------------------------------

    static sf::String DebugTextBallState( eBallState eBallState )
    {
        if (eBallState == eBallState::LEFT)
        {
            return sf::String("LEFT:");
        }
        else if (eBallState == eBallState::RIGHT)
        {
            return sf::String("RIGHT:");
        }
        else if (eBallState == eBallState::HitBall)
        {
            return sf::String("HitBall:");
        }
        else if (eBallState == eBallState::HitWall)
        {
            return sf::String("HitWall:");
        }
        else if (eBallState == eBallState::ResetGamePosition)
        {
            return sf::String("RESET:");
        }
        else
        {
            std::string sEnumValue = std::to_string(eBallState);
            return sf::String(sEnumValue);
        }
    }
};

