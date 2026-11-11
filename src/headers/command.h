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

protected:
  int m_iAnalogStrength = 100;
  bool m_bIsUsingPad = false;
  bool m_bIsUsingStick = false;
  bool m_bIsInverted = true;
};

class Command_Joystick : public Command
{
  virtual void SetIsUsingStick(bool bIsUsingStick) override { Command::SetIsUsingStick(true); }
};

//----------------------------------------------------------

class Command_ButtonPress : public Command
{
  virtual void SetIsUsingPad(bool bIsUsingPad) override { Command::SetIsUsingPad(true); }
};

//----------------------------------------------------------

class Player1DownCommand : public Command_ButtonPress
{
public:
  virtual void execute(const DataStruct& rTuple)
  {
    rTuple.pBat1->SetAnalogSpeedModifier(Command::GetAnalogStrength());
    rTuple.pBat1->SetDesiredMoveDirection(eBatMoveDirection::DOWN);
  }

};

//----------------------------------------------------------

class Player1UpCommand : public Command_ButtonPress
{
public:
  virtual void execute(const DataStruct& rTuple)
  {
    rTuple.pBat1->SetAnalogSpeedModifier(Command::GetAnalogStrength());
    rTuple.pBat1->SetDesiredMoveDirection(eBatMoveDirection::UP);
  }
};

//----------------------------------------------------------

class Player2DownCommand : public Command_ButtonPress
{
public:
  virtual void execute(const DataStruct& rTuple)
  {
    rTuple.pBat2->SetAnalogSpeedModifier(Command::GetAnalogStrength());
    rTuple.pBat2->SetDesiredMoveDirection(eBatMoveDirection::DOWN);
  }
};

//----------------------------------------------------------

class Player2UpCommand : public Command_ButtonPress
{
public:
  virtual void execute(const DataStruct& rTuple)
  {
    rTuple.pBat2->SetAnalogSpeedModifier(Command::GetAnalogStrength());
    rTuple.pBat2->SetDesiredMoveDirection(eBatMoveDirection::UP);
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
        rTuple.pWorldState->SetDesiredGamestate(eGameState::Boot);
    }
    else
    {
        rTuple.pWorldState->SetDesiredGamestate(eGameState::Paused);
    }
  }

};
//----------------------------------------------------------

class JoystickMovementCommand : public Command_Joystick
{
  public:
    virtual void execute(const DataStruct& rTuple)
    {
      rTuple.pBat1->SetAnalogSpeedModifier(m_iAnalogStrength);

      if (m_iAnalogStrength > 10)
      {
        rTuple.pBat1->SetDesiredMoveDirection(eBatMoveDirection::DOWN);
      }
      else if (m_iAnalogStrength < -10 )
      {
          rTuple.pBat1->SetDesiredMoveDirection(eBatMoveDirection::UP);
      }
      else
      {
        rTuple.pBat1->SetDesiredMoveDirection(eBatMoveDirection::NONE);
      }
    }
};

//----------------------------------------------------------

class Player2JoystickMovementCommand : public Command_Joystick
{
  public:
    virtual void execute(const DataStruct& rTuple)
    {
      rTuple.pBat2->SetAnalogSpeedModifier(m_iAnalogStrength);

      if (m_iAnalogStrength > 10)
      {
        rTuple.pBat2->SetDesiredMoveDirection(eBatMoveDirection::DOWN);
      }
      else if (m_iAnalogStrength < 10 )
      {
        rTuple.pBat2->SetDesiredMoveDirection(eBatMoveDirection::UP);
      }
      else
      {
        rTuple.pBat2->SetDesiredMoveDirection(eBatMoveDirection::NONE);
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
      //rTuple.~DataStruct();
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

class StopPlayer1DownCommand : public Command
{
public:
    virtual void execute(const DataStruct& rTuple )
    {
        if (rTuple.pBat1->GetCurrentMoveDirection() == eBatMoveDirection::DOWN)
        rTuple.pBat1->SetDesiredMoveDirection(eBatMoveDirection::NONE);
    }
};

//----------------------------------------------------------

class StopPlayer2DownCommand : public Command
{
public:
    virtual void execute( const DataStruct& rTuple )
    {
        if (rTuple.pBat2->GetCurrentMoveDirection() == eBatMoveDirection::DOWN)
        rTuple.pBat2->SetDesiredMoveDirection(eBatMoveDirection::NONE);
    }
};

//----------------------------------------------------------

class ReleaseJoystickCommand : public Command
{
public:
    virtual void execute( const DataStruct& rTuple )
    {
      rTuple.pBat1->SetDesiredMoveDirection(eBatMoveDirection::NONE);
    }
};

//----------------------------------------------------------

class P2ReleaseJoystickCommand : public Command
{
public:
    virtual void execute( const DataStruct& rTuple )
    {
      rTuple.pBat2->SetDesiredMoveDirection(eBatMoveDirection::NONE);
    }
};

//----------------------------------------------------------

class Player1ButtonShootCommand : public Command_Joystick
{
public:
  virtual void execute(const DataStruct& rTuple )
  {
    if (rTuple.pWorldState->GetCurrentGameState() == eGameState::Paused)
    {
      rTuple.pWorldState->SetDesiredGamestate(eGameState::Running);
    }

    if (rTuple.pWorldState->GetCurrentGameState() == eGameState::GameOver)
    {
      rTuple.pWorldState->SetDesiredGamestate(eGameState::Reset);
    }

    if (rTuple.pBall->GetCurrentBallState() == eBallState::AtPlayer1)
    {
       rTuple.pBat1->SetLastHitFrame(rTuple.pWorldState->GetCurrentSimFrame());
       rTuple.pBall->SetDesiredBallState(eBallState::RIGHT);
       rTuple.pBall->StateMachine();
       if (rTuple.pWorldState->GetShouldPlaySFX())
       {
          rTuple.pSpitBallSoundEffect->play();
       }
    }
  }
};

//----------------------------------------------------------

class Player2ButtonShootCommand : public Command
{
public:
  virtual void execute(const DataStruct& rTuple )
  {
    if (rTuple.pWorldState->GetCurrentGameState() == eGameState::Paused)
    {
      rTuple.pWorldState->SetDesiredGamestate(eGameState::Running);
    }

    if (rTuple.pWorldState->GetCurrentGameState() == eGameState::GameOver)
    {
      rTuple.pWorldState->SetDesiredGamestate(eGameState::Reset);
    }

    if (rTuple.pBall->GetCurrentBallState() == eBallState::AtPlayer2)
    {
       rTuple.pBat2->SetLastHitFrame(rTuple.pWorldState->GetCurrentSimFrame());
       rTuple.pBall->SetDesiredBallState(eBallState::LEFT);
       rTuple.pBall->StateMachine();
       if (rTuple.pWorldState->GetShouldPlaySFX())
       {
          rTuple.pSpitBallSoundEffect->play();
       }
    }
  }
};

//----------------------------------------------------------

class StopPlayer1UpCommand : public Command
{
public:
    virtual void execute( const DataStruct& rTuple )
    {
        if (rTuple.pBat1->GetCurrentMoveDirection() == eBatMoveDirection::UP)
        rTuple.pBat1->SetDesiredMoveDirection(eBatMoveDirection::NONE);
    }
};

//----------------------------------------------------------

class StopPlayer2UpCommand : public Command
{
public:
    virtual void execute( const DataStruct& rTuple )
    {
        if (rTuple.pBat2->GetCurrentMoveDirection() == eBatMoveDirection::UP)
        rTuple.pBat2->SetDesiredMoveDirection(eBatMoveDirection::NONE);
    }
};

//----------------------------------------------------------

