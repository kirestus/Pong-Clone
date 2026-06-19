#include <headers/alias.h>

class PadUtils
{
    public:
    bool IsConnected(uint8);
    bool IsActionButtonPressed();

    uint8 GetJoystickID() const {return m_iJoystickID ;}
    
    private:

    uint8 m_iJoystickID = 0;
    uint8 m_iPlayerNumber = 0;
};