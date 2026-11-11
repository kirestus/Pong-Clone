#include <headers/dataStruct.h>
#include <headers/debugText.h>
#include <headers/alias.h>
#include <iostream>

class Command
{  
public:
    virtual ~Command() {std::cout<<"Destroying Command"<<this;}
    virtual void execute(const DataStruct& rTuple) = 0;
    virtual void SetAnalogStrength(int val, bool bInvert){m_iAnalogStrength = val,m_bIsInverted = bInvert;}
    virtual int GetAnalogStrength() const { return m_iAnalogStrength; }
    virtual void SetIsUsingPad(bool bIsUsingPad){m_bIsUsingPad = bIsUsingPad ;}
    virtual void SetIsUsingStick(bool bIsUsingStick ){m_bIsUsingStick = bIsUsingStick ;}
    virtual bool GetIsInverted()const {return m_bIsInverted ; }

private:
  int m_iAnalogStrength = 100;
  bool m_bIsUsingPad = false;
  bool m_bIsUsingStick = false;
  bool m_bIsInverted = true;
};

//----------------------------------------------------------

class Command_Joystick : public Command
{
  virtual void SetIsUsingStick(bool bIsUsingStick) override { Command::SetIsUsingStick(true); }
  
  public: 
    Command_Joystick(ePlayerNumber eBatNumber){ m_ePlayer = eBatNumber; }
    virtual float GetAnalogStickDeadzone() const { return m_iAnalogDeadzone ;}
    virtual ePlayerNumber GetPlayerNumberEnum() const { return m_ePlayer; }

  protected:
    int m_iAnalogDeadzone = 10;
    ePlayerNumber m_ePlayer = ePlayerNumber::PLAYER1;
};

//----------------------------------------------------------

//TODO: dont want this in a header file
class JoystickMovementCommand : public Command_Joystick
{
  public:
    JoystickMovementCommand(ePlayerNumber eBatNumber) : Command_Joystick(eBatNumber){}

    virtual void execute(const DataStruct& rTuple)
    {

      std::shared_ptr<Bat> pBattPointer = GetPlayerNumberEnum() == ePlayerNumber::PLAYER1 ? rTuple.pBat1 : rTuple.pBat2;

      pBattPointer->SetAnalogSpeedModifier(GetAnalogStrength());

      if (GetAnalogStrength() > GetAnalogStickDeadzone())
      {
        pBattPointer->SetDesiredMoveDirection(eBatMoveDirection::DOWN);
      }
      else if (GetAnalogStrength() < -1 * GetAnalogStickDeadzone() )
      {
          pBattPointer->SetDesiredMoveDirection(eBatMoveDirection::UP);
      }
      else
      {
        pBattPointer->SetDesiredMoveDirection(eBatMoveDirection::NONE);
      }
    }
};

//----------------------------------------------------------

class ReleaseJoystickCommand : public Command_Joystick
{

    public:
    ReleaseJoystickCommand(ePlayerNumber eBatNumber) : Command_Joystick(eBatNumber){}

    virtual void execute( const DataStruct& rTuple )
    {
      std::shared_ptr<Bat> pBattPointer = GetPlayerNumberEnum() == ePlayerNumber::PLAYER1 ? rTuple.pBat1 : rTuple.pBat2;
      pBattPointer->SetDesiredMoveDirection(eBatMoveDirection::NONE);
    }
};

//----------------------------------------------------------

class Command_ButtonPress : public Command
{
  public:
    //Command_ButtonPress(){};
    Command_ButtonPress(ePlayerNumber eBatNumber, eBatMoveDirection eDirection){ m_ePlayer = eBatNumber; m_eDirection = eDirection ;}

    virtual void SetIsUsingPad(bool bIsUsingPad) override { Command::SetIsUsingPad(true); }
    virtual ePlayerNumber GetPlayerNumberEnum() const { return m_ePlayer; }

    protected:
    ePlayerNumber m_ePlayer = ePlayerNumber::PLAYER1;
    eBatMoveDirection m_eDirection = eBatMoveDirection::NONE;
};

//----------------------------------------------------------

class ShootButtonCommand : public Command_ButtonPress
{
public:
  ShootButtonCommand(ePlayerNumber eBatNumber, eBatMoveDirection eDirection)
    : Command_ButtonPress(eBatNumber, eDirection) {}

  virtual void execute(const DataStruct& rTuple )
  {
    std::shared_ptr<Bat> pBattPointer = GetPlayerNumberEnum() == ePlayerNumber::PLAYER1 ? rTuple.pBat1 : rTuple.pBat2;

    if (rTuple.pWorldState->GetCurrentGameState() == eGameState::Paused)
    {
      rTuple.pWorldState->SetDesiredGamestate(eGameState::Running);
    }

    if (rTuple.pWorldState->GetCurrentGameState() == eGameState::GameOver)
    {
      rTuple.pWorldState->SetDesiredGamestate(eGameState::Reset);
    }

    if (rTuple.pBall->GetCurrentBallState() == eBallState::AtPlayer1 || rTuple.pBall->GetCurrentBallState() == eBallState::AtPlayer2)
    {
      pBattPointer->SetLastHitFrame(rTuple.pWorldState->GetCurrentSimFrame());
      
      const bool bIsPlayerOneHoldingBall = m_ePlayer == ePlayerNumber::PLAYER1;

      if(bIsPlayerOneHoldingBall)
      {
        rTuple.pBall->SetDesiredBallState(eBallState::RIGHT);
      }
      else
      {
        rTuple.pBall->SetDesiredBallState(eBallState::LEFT);
      }

      rTuple.pBall->StateMachine();

      //not crazy about playing the sound effect based on the input::
      if (rTuple.pWorldState->GetShouldPlaySFX())
      {
        rTuple.pSpitBallSoundEffect->play();
      }
    }
  }
};

//----------------------------------------------------------

class InputDownCommand : public Command_ButtonPress
{
  
public:
  InputDownCommand(ePlayerNumber eBatNumber, eBatMoveDirection eDirection) : Command_ButtonPress(eBatNumber,eDirection){}

  virtual void execute(const DataStruct& rTuple)
  {
    std::shared_ptr<Bat> pBattPointer = GetPlayerNumberEnum() == ePlayerNumber::PLAYER1 ? rTuple.pBat1 : rTuple.pBat2;

    pBattPointer->SetAnalogSpeedModifier(Command::GetAnalogStrength());
    pBattPointer->SetDesiredMoveDirection(m_eDirection);
  }

};

//----------------------------------------------------------

class ReleaseInputCommand : public Command_ButtonPress
{
public:
  ReleaseInputCommand(ePlayerNumber eBatNumber, eBatMoveDirection eDirection) : Command_ButtonPress(eBatNumber,eDirection){}

  virtual void execute(const DataStruct& rTuple)
  {
    std::shared_ptr<Bat> pBattPointer = GetPlayerNumberEnum() == ePlayerNumber::PLAYER1 ? rTuple.pBat1 : rTuple.pBat2;

      if (pBattPointer->GetCurrentMoveDirection() == m_eDirection)
      pBattPointer->SetDesiredMoveDirection(eBatMoveDirection::NONE);
  }
};

//----------------------------------------------------------

class StartCommand : public Command
{
public:
  virtual void execute(const DataStruct& rTuple)
  {
    if ( rTuple.pWorldState->GetCurrentGameState() == eGameState::Paused )
    {
        rTuple.pWorldState->SetDesiredGamestate(eGameState::Running);
    }
    else if (rTuple.pWorldState->GetCurrentGameState() == eGameState::GameOver)
    {
        rTuple.pWorldState->SetDesiredGamestate(eGameState::Reset);
    }
    else
    {
        rTuple.pWorldState->SetDesiredGamestate(eGameState::Paused);
    }
  }

};
//----------------------------------------------------------

class DebugTextGameStateCommand : public Command
{
public:
  virtual void execute(const DataStruct& rTuple)
  {

      rTuple.pDebugText->SetDebuggingGamelState(!rTuple.pDebugText->GetIsDebuggingGameState());
      rTuple.pDebugText->SetDebuggingBallState(false);
  }
};

//----------------------------------------------------------

class DebugTextBallStateCommand : public Command
{
public:
  virtual void execute(const DataStruct& rTuple)
  {
    rTuple.pDebugText->SetDebuggingBallState(!rTuple.pDebugText->GetIsDebuggingBallState());
    rTuple.pDebugText->SetDebuggingGamelState(false);
  }
};

//----------------------------------------------------------

class PauseMusicCommand : public Command
{
public:
  virtual void execute(const DataStruct& rTuple)
  {
    if (rTuple.pGameMusic->Playing)
    {
      rTuple.pGameMusic->pause();
    }
    else if (rTuple.pGameMusic->Stopped || rTuple.pGameMusic->Paused)
    {
      rTuple.pGameMusic->play();
    }
  }
};

//----------------------------------------------------------

class UnlockFPS : public Command
{
public:
  virtual void execute(const DataStruct& rTuple)
  {
    //TODO: Add this feature
  }
};

//----------------------------------------------------------

class ToggleSFXCommand : public Command
{
public:
  virtual void execute(const DataStruct& rTuple)
  {
    const bool toggleSFX = !rTuple.pWorldState->GetShouldPlaySFX();
    rTuple.pWorldState->SetShouldPlaySFX(toggleSFX);
  }
};

//----------------------------------------------------------

class QuitCommand : public Command
{
public:
  virtual void execute(const DataStruct& rTuple)
  {
      rTuple.pRenderWindow->close();
  }
};

//----------------------------------------------------------


class ResetGameCommand : public Command
{
public:
  virtual void execute(const DataStruct& rTuple)
  {
    rTuple.pWorldState->SetDesiredGamestate(eGameState::Reset);
  }
};


//----------------------------------------------------------



//----------------------------------------------------------

