#include <SFML/Graphics.hpp>
#include <string>
#include <headers/gameState.h>
#include <headers/eBallstate.h>


class DebugText
{
public:
    static sf::String DebugTextGameState( eGameState eGameState );
    static sf::String DebugTextBallState( eBallState eBallState );
};
