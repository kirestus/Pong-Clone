#include <headers/inputHandler.h>
#include <SFML/System.hpp>




Command* InputHandler::HandleInput( sf::Event* pEvent, Bat* pBat )
{
    if(pEvent->type == sf::Event::KeyPressed )
    {
        if ( pEvent->key.code ==  sf::Keyboard::W )
        return pPlayer1ButtonUp_;

        else if ( pEvent->key.code ==  sf::Keyboard::Up ) 
        return pPlayer2ButtonUp_;

        else if ( pEvent->key.code ==  sf::Keyboard::S ) 
        return pPlayer1ButtonDown_;
        
        else if ( pEvent->key.code ==  sf::Keyboard::Down )
        return pPlayer2ButtonDown_;
    }

    if (pEvent->type == pEvent->KeyReleased)
    {
        if (pEvent->key.code ==  sf::Keyboard::W)
        return pReleasePlayer1ButtonUp_;

        else if (pEvent->key.code ==  sf::Keyboard::Up )
        return pReleasePlayer2ButtonUp_;

        else if (pEvent->key.code ==  sf::Keyboard::S)
        return pReleasePlayer1ButtonDown_;

        else if ( pEvent->key.code ==  sf::Keyboard::Down )
        return pReleasePlayer2ButtonDown_;
    }  



    //Remove this after i do the next part
    return NULL;

}

