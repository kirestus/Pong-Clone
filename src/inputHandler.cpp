#include <headers/inputHandler.h>

InputHandler::InputHandler()
{
    //add all of the commands to an array
    m_aCommandArray[0] = m_pPlayer1ButtonUp_;
    m_aCommandArray[1] = m_pPlayer1ButtonDown_;
    m_aCommandArray[2] = m_pPlayer2ButtonUp_;
    m_aCommandArray[3] = m_pPlayer2ButtonDown_;
    m_aCommandArray[4] = m_pReleasePlayer1ButtonUp_;
    m_aCommandArray[5] = m_pReleasePlayer2ButtonUp_;
    m_aCommandArray[6] = m_pReleasePlayer1ButtonDown_;
    m_aCommandArray[7] = m_pReleasePlayer2ButtonDown_;
    m_aCommandArray[8] = m_pStartGame_;
    m_aCommandArray[9] = m_pQuitGame_;
}


InputHandler::~InputHandler()
{
    //clean up all the commands on the heap when the input handler is destroyed
    for(Command* command : m_aCommandArray)
    {
        delete command;
    }
}

Command* InputHandler::HandleInput( sf::Event* pEvent )
{
    if(pEvent->type == sf::Event::KeyPressed )
    {
        if ( pEvent->key.code ==  sf::Keyboard::W )
        return m_pPlayer1ButtonUp_;

        else if ( pEvent->key.code ==  sf::Keyboard::Up ) 
        return m_pPlayer2ButtonUp_;

        else if ( pEvent->key.code ==  sf::Keyboard::S ) 
        return m_pPlayer1ButtonDown_;
        
        else if ( pEvent->key.code ==  sf::Keyboard::Down )
        return m_pPlayer2ButtonDown_;

        else if ( pEvent->key.code == sf::Keyboard::Space)
        return m_pStartGame_;

        else if ( pEvent->key.code == sf::Keyboard::Escape)
        return m_pQuitGame_;
    }

    if (pEvent->type == pEvent->KeyReleased)
    {
        if (pEvent->key.code ==  sf::Keyboard::W)
        return m_pReleasePlayer1ButtonUp_;

        else if (pEvent->key.code ==  sf::Keyboard::Up )
        return m_pReleasePlayer2ButtonUp_;

        else if (pEvent->key.code ==  sf::Keyboard::S)
        return m_pReleasePlayer1ButtonDown_;

        else if ( pEvent->key.code ==  sf::Keyboard::Down )
        return m_pReleasePlayer2ButtonDown_;
    }  

    //Remove this after i do the next part
    return NULL;

}

