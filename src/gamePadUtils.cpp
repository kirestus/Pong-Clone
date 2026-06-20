#include <headers/gamePadUtils.h>
#include <SFML/Window.hpp>
#include <iostream>

enum PadButtons : uint8
{
    //the order of these matters
    ACTION_BUTTON1,
    ACTION_BUTTON2,
    ACTION_BUTTON3,
    ACTION_BUTTON4,
    DEBUG_BUTTON,
    PAUSE_BUTTON,
    EXIT_BUTTON,
    length,
};

bool PadUtils::IsConnected(uint8 iPadID)
{
    return sf::Joystick::isConnected(iPadID);
}

bool PadUtils::IsActionButtonPressed()
{
    for (uint8 i = PadButtons::length; i >= 0; i--)
    {
        if (sf::Joystick::isButtonPressed(GetJoystickID(),i) && i <= ACTION_BUTTON4)
        {
            return true;
        }
    }
    return false;
}



