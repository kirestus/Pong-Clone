#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <headers/gameState.h>
#include <headers/eBallstate.h>
#include <headers/alias.h>


class DebugText
{
public:
    static sf::String DebugTextGameState( eGameState eGameState );
    static sf::String DebugTextBallState( eBallState eBallState );


    bool GetIsDebuggingBallState()const {return m_bIsDebuggingBallState; }
    void SetDebuggingBallState( bool isDebugging ){m_bIsDebuggingBallState = isDebugging ;}

    bool GetIsDebuggingGameState() const {return m_bIsDebuggingGameState; }
    void SetDebuggingGamelState( bool isDebugging ){m_bIsDebuggingGameState = isDebugging ;}

private:
    bool m_bIsDebuggingGameState = false;
    bool m_bIsDebuggingBallState = false;

};
