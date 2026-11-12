#pragma once
#include <headers/command.h>

class InputHandler
{
public:
    InputHandler();
    ~InputHandler();
    Command_Input* GetCommandArray()const{return *m_aCommandArray ;}
    Command_Input* HandleInput( sf::Event* pEvent, bool );

private:
    Command_Input* m_pPlayer1ButtonUp_ = new InputDownCommand(ePlayerNumber::PLAYER1,eBatMoveDirection::UP); 
    Command_Input* m_pPlayer1ButtonDown_ = new InputDownCommand(ePlayerNumber::PLAYER1,eBatMoveDirection::DOWN); 
    Command_Input* m_pPlayer2ButtonUp_ = new InputDownCommand(ePlayerNumber::PLAYER2,eBatMoveDirection::UP);
    Command_Input* m_pPlayer2ButtonDown_ = new InputDownCommand(ePlayerNumber::PLAYER2,eBatMoveDirection::DOWN);

    Command_Input* m_pReleasePlayer1ButtonUp_ = new ReleaseInputCommand(ePlayerNumber::PLAYER1, eBatMoveDirection::UP);
    Command_Input* m_pReleasePlayer2ButtonUp_ = new ReleaseInputCommand(ePlayerNumber::PLAYER2, eBatMoveDirection::UP);; 
    Command_Input* m_pReleasePlayer1ButtonDown_ = new ReleaseInputCommand(ePlayerNumber::PLAYER1,eBatMoveDirection::DOWN); 
    Command_Input* m_pReleasePlayer2ButtonDown_ = new ReleaseInputCommand(ePlayerNumber::PLAYER2,eBatMoveDirection::DOWN); ;
    
    Command_Input* m_pJoystickMovement = new JoystickMovementCommand(ePlayerNumber::PLAYER1);
    Command_Input* m_pReleaseJoyStick_ = new ReleaseJoystickCommand(ePlayerNumber::PLAYER1) ;

    Command_Input* m_pP2JoystickMovement = new JoystickMovementCommand(ePlayerNumber::PLAYER2);
    Command_Input* m_pP2ReleaseJoyStick_ = new ReleaseJoystickCommand(ePlayerNumber::PLAYER2);

    Command_Input* m_pPlayer1ButtonShoot = new ShootButtonCommand(ePlayerNumber::PLAYER1, eBatMoveDirection::NONE);
    Command_Input* m_pPlayer2ButtonShoot = new ShootButtonCommand(ePlayerNumber::PLAYER2, eBatMoveDirection::NONE);;
    
    Command_Input* m_pPressKeyNum1 = new ToggleSFXCommand;
    Command_Input* m_pPressKeyNum2 = new DebugTextBallStateCommand ;
    Command_Input* m_pPressKeyNum3 = new DebugTextGameStateCommand;
    Command_Input* m_pPressKeyNum4 = new PauseMusicCommand ;
    Command_Input* m_pPressKeyNum5 = new PauseMusicCommand ;
    Command_Input* m_pPressKeyNum0 = new ResetGameCommand ;
    
    Command_Input* m_pPressSpace = new StartCommand ;
    Command_Input* m_pQuitGame_ = new QuitCommand ;

    Command_Input* m_pPressActionButton = new ShootButtonCommand(ePlayerNumber::PLAYER1, eBatMoveDirection::NONE) ;
    Command_Input* m_pPressPauseButton = new StartCommand ;

    Command_Input* m_aCommandArray[20];
};