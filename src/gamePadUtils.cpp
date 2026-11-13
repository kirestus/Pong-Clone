#include <headers/gamePadUtils.h>
#include <SFML/Window.hpp>

enum PadButtons : uint8
{
    //the order of these matters
    ACTION_BUTTON1 = 0,
    ACTION_BUTTON2 = 1,
    ACTION_BUTTON3 = 2,
    ACTION_BUTTON4 = 3,
    DEBUG_BUTTON = 4,
    PAUSE_BUTTON = 5,
    EXIT_BUTTON = 6,
    length = 7,
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



