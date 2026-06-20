#include <headers/inputHandler.h>
#include <iostream>

static constexpr int g_iStickDeadZone = 20;

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
    m_aCommandArray[8] = m_pPressSpace;
    m_aCommandArray[9] = m_pQuitGame_;
    m_aCommandArray[10] = m_pPressKeyNum1;
    m_aCommandArray[11] = m_pPressKeyNum2;
    m_aCommandArray[12] = m_pPressKeyNum4;
    m_aCommandArray[13] = m_pPressKeyNum5;
    m_aCommandArray[14] = m_pPressKeyNum0;
    m_aCommandArray[15] = m_pPlayer1ButtonShoot;
    m_aCommandArray[16] = m_pPlayer2ButtonShoot;
    m_aCommandArray[17] = m_pReleaseJoyStick_;
    m_aCommandArray[18] = m_pPressActionButton;
    m_aCommandArray[19] = m_pPressPauseButton;

}


InputHandler::~InputHandler()
{
    //clean up all the commands on the heap when the input handler is destroyed
    for(Command* command : m_aCommandArray)
    {
        delete command;
    }
}

Command* InputHandler::HandleInput( sf::Event* pEvent, bool bIsWinConditionMet )
{
    if (pEvent->type == sf::Event::KeyPressed )
    {
        if ( pEvent->key.code == sf::Keyboard::Escape)
        return m_pQuitGame_;

        else if ( pEvent->key.code == sf::Keyboard::Num1)
        return m_pPressKeyNum1;

        else if ( pEvent->key.code == sf::Keyboard::Num2)
        return m_pPressKeyNum2;

        else if ( pEvent->key.code == sf::Keyboard::Num3)
        return m_pPressKeyNum3;

        else if ( pEvent->key.code == sf::Keyboard::Num4)
        return m_pPressKeyNum4;

        else if ( pEvent->key.code == sf::Keyboard::Num5)
        return m_pPressKeyNum5;

        else if (pEvent->key.code == sf::Keyboard::Num0)
        return m_pPressKeyNum0;
    }

    //not an else if because both conditions will be true 
    if(pEvent->type == sf::Event::KeyPressed && !bIsWinConditionMet )
    {
        if ( pEvent->key.code ==  sf::Keyboard::W )
        {
            m_pPlayer1ButtonUp_->SetAnalogStrength(100);
            return m_pPlayer1ButtonUp_;
        }

        else if ( pEvent->key.code ==  sf::Keyboard::Up ) 
        {
            m_pPlayer2ButtonUp_->SetAnalogStrength(100);
            return m_pPlayer2ButtonUp_;
        }

        else if ( pEvent->key.code ==  sf::Keyboard::S )
        {
            m_pPlayer1ButtonDown_->SetAnalogStrength(100); 
            return m_pPlayer1ButtonDown_;
        }
        
        else if ( pEvent->key.code ==  sf::Keyboard::Down )
        {
            m_pPlayer1ButtonDown_->SetAnalogStrength(100); 
            return m_pPlayer2ButtonDown_;
        }

        else if ( pEvent->key.code == sf::Keyboard::Space)
        return m_pPressSpace;

        else if ( pEvent->key.code == sf::Keyboard::F)
        return m_pPlayer1ButtonShoot;

        else if ( pEvent->key.code == sf::Keyboard::Slash)
        return m_pPlayer2ButtonShoot;

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


    //Handle joystick inputs 

    if (pEvent->type == pEvent->JoystickMoved && pEvent->joystickMove.axis == sf::Joystick::Axis::Y)
    {
        const bool bIsLeftStick = pEvent->joystickMove.axis == sf::Joystick::Axis::Y;

        if(!bIsLeftStick)
        return m_pReleaseJoyStick_;

        m_pJoystickMovement->SetAnalogStrength(pEvent->joystickMove.position);
        if (pEvent->joystickMove.position < -g_iStickDeadZone)
        return m_pJoystickMovement;

        else if (pEvent->joystickMove.position > g_iStickDeadZone)
        return m_pJoystickMovement;

        else
        return m_pReleaseJoyStick_;
    }

    if (pEvent->type == pEvent->JoystickButtonPressed)
    {
        std::cout<<"Controller "<< pEvent->joystickButton.joystickId << " Pressed the "<< pEvent->joystickButton.button <<" Button \n";
        if(pEvent->joystickButton.button == 0)
        return m_pPressActionButton;
        
        if(pEvent->joystickButton.button == 1)
        return m_pPressActionButton;
            
        else if(pEvent->joystickButton.button == 2)
        return m_pPressActionButton;

        else if(pEvent->joystickButton.button == 3)
        return m_pPressActionButton;

        else if(pEvent->joystickButton.button == 4)
        return m_pPressActionButton;

        else if(pEvent->joystickButton.button == 5)
        return m_pPressActionButton;

        else if(pEvent->joystickButton.button == 6)
          return m_pPressActionButton;

        else if(pEvent->joystickButton.button == 7)
        return m_pPressPauseButton;

        else if(pEvent->joystickButton.button == 8)
        return m_pPressActionButton;

    }

    if (pEvent->type == pEvent->JoystickButtonReleased)
    {
        if(pEvent->joystickButton.button == 12)
            return m_pReleasePlayer1ButtonUp_;
        
        else if(pEvent->joystickButton.button == 13)
        return m_pReleasePlayer1ButtonDown_;
    }

    
    return NULL;

}

