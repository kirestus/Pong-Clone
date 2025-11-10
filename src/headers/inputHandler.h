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
    Command* m_pPlayer1ButtonUp_ = new Player1UpCommand; 
    Command* m_pPlayer1ButtonDown_ = new Player1DownCommand; 
    Command* m_pPlayer2ButtonUp_ = new Player2UpCommand; 
    Command* m_pPlayer2ButtonDown_ = new Player2DownCommand; 
    Command* m_pReleasePlayer1ButtonUp_ = new StopPlayer1UpCommand ;
    Command* m_pReleasePlayer2ButtonUp_ = new StopPlayer2UpCommand; 
    Command* m_pReleasePlayer1ButtonDown_ = new StopPlayer1DownCommand; 
    Command* m_pReleasePlayer2ButtonDown_ = new StopPlayer2DownCommand ;

    Command* m_pPlayer1ButtonShoot = new Player1ButtonShootCommand;
    Command* m_pPlayer2ButtonShoot = new Player2ButtonShootCommand;
    
    Command* m_pPressKeyNum1 = new DebugTextGameStateCommand ;
    Command* m_pPressKeyNum2 = new DebugTextBallStateCommand ;
    Command* m_pPressKeyNum3 = new PauseMusicCommand ;
    Command* m_pPressKeyNum4 = new PauseMusicCommand ;
    Command* m_pPressKeyNum5 = new PauseMusicCommand ;
    Command* m_pPressKeyNum0 = new ResetGameCommand ;
    
    Command* m_pPressSpace = new StartCommand ;
    Command* m_pQuitGame_ = new QuitCommand ;

    Command* m_aCommandArray[17];
};