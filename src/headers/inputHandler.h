#pragma once
#include <headers/command.h>

class InputHandler
{
public:
    InputHandler();
    ~InputHandler();
    Command* HandleInput( sf::Event* pEvent, Bat* pBat );

private:
    Command* m_pPlayer1ButtonUp_ = nullptr; 
    Command* m_pPlayer1ButtonDown_ = nullptr; ;
    Command* m_pPlayer2ButtonUp_ = nullptr; ;
    Command* m_pPlayer2ButtonDown_ = nullptr; ;
    Command* m_pReleasePlayer1ButtonUp_ = nullptr; ;
    Command* m_pReleasePlayer2ButtonUp_ = nullptr; ;
    Command* m_pReleasePlayer1ButtonDown_ = nullptr; ;
    Command* m_pReleasePlayer2ButtonDown_ = nullptr; ;

    Command* m_pStartGame_ = nullptr; ;
    Command* m_pQuitGame_ = nullptr; ;

    Command* m_aCommandArray[10];
};