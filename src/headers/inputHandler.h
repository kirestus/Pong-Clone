#pragma once
#include <headers/command.h>

class InputHandler
{
public:
    InputHandler();
    ~InputHandler();
    Command* GetCommandArray()const{return *m_aCommandArray ;}
    Command* HandleInput( sf::Event* pEvent, bool );

private:
    Command* m_pPlayer1ButtonUp_ = new InputDownCommand(ePlayerNumber::PLAYER1,eBatMoveDirection::UP); 
    Command* m_pPlayer1ButtonDown_ = new InputDownCommand(ePlayerNumber::PLAYER1,eBatMoveDirection::DOWN); 
    Command* m_pPlayer2ButtonUp_ = new InputDownCommand(ePlayerNumber::PLAYER2,eBatMoveDirection::UP);
    Command* m_pPlayer2ButtonDown_ = new InputDownCommand(ePlayerNumber::PLAYER2,eBatMoveDirection::DOWN);

    Command* m_pReleasePlayer1ButtonUp_ = new ReleaseInputCommand(ePlayerNumber::PLAYER1, eBatMoveDirection::UP);
    Command* m_pReleasePlayer2ButtonUp_ = new ReleaseInputCommand(ePlayerNumber::PLAYER2, eBatMoveDirection::UP);; 
    Command* m_pReleasePlayer1ButtonDown_ = new ReleaseInputCommand(ePlayerNumber::PLAYER1,eBatMoveDirection::DOWN); 
    Command* m_pReleasePlayer2ButtonDown_ = new ReleaseInputCommand(ePlayerNumber::PLAYER2,eBatMoveDirection::DOWN); ;
    
    Command* m_pJoystickMovement = new JoystickMovementCommand(ePlayerNumber::PLAYER1);
    Command* m_pReleaseJoyStick_ = new ReleaseJoystickCommand(ePlayerNumber::PLAYER1) ;

    Command* m_pP2JoystickMovement = new JoystickMovementCommand(ePlayerNumber::PLAYER2);
    Command* m_pP2ReleaseJoyStick_ = new ReleaseJoystickCommand(ePlayerNumber::PLAYER2);

    Command* m_pPlayer1ButtonShoot = new ShootButtonCommand(ePlayerNumber::PLAYER1, eBatMoveDirection::NONE);
    Command* m_pPlayer2ButtonShoot = new ShootButtonCommand(ePlayerNumber::PLAYER2, eBatMoveDirection::NONE);;
    
    Command* m_pPressKeyNum1 = new ToggleSFXCommand;
    Command* m_pPressKeyNum2 = new DebugTextBallStateCommand ;
    Command* m_pPressKeyNum3 = new DebugTextGameStateCommand;
    Command* m_pPressKeyNum4 = new PauseMusicCommand ;
    Command* m_pPressKeyNum5 = new PauseMusicCommand ;
    Command* m_pPressKeyNum0 = new ResetGameCommand ;
    
    Command* m_pPressSpace = new StartCommand ;
    Command* m_pQuitGame_ = new QuitCommand ;

    Command* m_pPressActionButton = new ShootButtonCommand(ePlayerNumber::PLAYER1, eBatMoveDirection::NONE) ;
    Command* m_pPressPauseButton = new StartCommand ;

    Command* m_aCommandArray[20];
};