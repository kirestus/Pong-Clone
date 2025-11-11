#include <headers/dataStruct.h>
#include <headers/debugText.h>

class Command
{  
public:
    virtual ~Command() {std::cout<<"Destroying Command";}
    virtual void execute(DataStruct& rTuple) = 0;
};
//----------------------------------------------------------

class Player1DownCommand : public Command
{
public:
  virtual void execute(DataStruct& rTuple)
  {
    rTuple.pBat1->SetDesiredMoveDirection(eBatMoveDirection::DOWN);
  }
};

//----------------------------------------------------------

class Player1UpCommand : public Command
{
public:
  virtual void execute(DataStruct& rTuple)
  {
    rTuple.pBat1->SetDesiredMoveDirection(eBatMoveDirection::UP);
  }
};

//----------------------------------------------------------

class Player2DownCommand : public Command
{
public:
  virtual void execute(DataStruct& rTuple)
  {
    rTuple.pBat2->SetDesiredMoveDirection(eBatMoveDirection::DOWN);
  }
};

//----------------------------------------------------------

class Player2UpCommand : public Command
{
public:
  virtual void execute(DataStruct& rTuple)
  {
    rTuple.pBat2->SetDesiredMoveDirection(eBatMoveDirection::UP);
  }
};

//----------------------------------------------------------

class StartCommand : public Command
{
public:
  virtual void execute(DataStruct& rTuple)
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

class DebugTextGameStateCommand : public Command
{
public:
  virtual void execute(DataStruct& rTuple)
  {
    rTuple.pMessageText->setString(DebugText::DebugTextGameState(rTuple.pWorldState->GetCurrentGameState()));
  }
};

//----------------------------------------------------------

class DebugTextBallStateCommand : public Command
{
public:
  virtual void execute(DataStruct& rTuple)
  {
    rTuple.pMessageText->setString(DebugText::DebugTextBallState(rTuple.pBall->GetCurrentBallState()));
  }
};

//----------------------------------------------------------

class PauseMusicCommand : public Command
{
public:
  virtual void execute(DataStruct& rTuple)
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
  virtual void execute(DataStruct& rTuple)
  {
    
  }
};

//----------------------------------------------------------

class QuitCommand : public Command
{
public:
  virtual void execute(DataStruct& rTuple)
  {
      rTuple.pRenderWindow->close();
      //rTuple.~DataStruct();
  }
};

//----------------------------------------------------------


class ResetGameCommand : public Command
{
public:
  virtual void execute(DataStruct& rTuple)
  {
    rTuple.pWorldState->SetDesiredGamestate(eGameState::Reset);
  }
};

//----------------------------------------------------------

class StopPlayer1DownCommand : public Command
{
public:
    virtual void execute( DataStruct& rTuple )
    {
        if (rTuple.pBat1->GetCurrentMoveDirection() == eBatMoveDirection::DOWN)
        rTuple.pBat1->SetDesiredMoveDirection(eBatMoveDirection::NONE);
    }
};

//----------------------------------------------------------

class StopPlayer2DownCommand : public Command
{
public:
    virtual void execute( DataStruct& rTuple )
    {
        if (rTuple.pBat2->GetCurrentMoveDirection() == eBatMoveDirection::DOWN)
        rTuple.pBat2->SetDesiredMoveDirection(eBatMoveDirection::NONE);
    }
};

//----------------------------------------------------------

class Player1ButtonShootCommand : public Command
{
public:
  virtual void execute( DataStruct& rTuple )
  {
    if (rTuple.pBall->GetCurrentBallState() == eBallState::AtPlayer1)
    {
       rTuple.pBall->SetDesiredBallState(eBallState::RIGHT);
       rTuple.pBall->StateMachine(rTuple.fScreenWidth);
       rTuple.pSpitBallSoundEffect->play();
    }
  }
};

//----------------------------------------------------------

class Player2ButtonShootCommand : public Command
{
public:
  virtual void execute( DataStruct& rTuple )
  {
    if (rTuple.pBall->GetCurrentBallState() == eBallState::AtPlayer2)
    {
       rTuple.pBall->SetDesiredBallState(eBallState::LEFT);
       rTuple.pBall->StateMachine(rTuple.fScreenWidth);
       rTuple.pSpitBallSoundEffect->play();  
    }
  }
};

//----------------------------------------------------------

class StopPlayer1UpCommand : public Command
{
public:
    virtual void execute( DataStruct& rTuple )
    {
        if (rTuple.pBat1->GetCurrentMoveDirection() == eBatMoveDirection::UP)
        rTuple.pBat1->SetDesiredMoveDirection(eBatMoveDirection::NONE);
    }
};

//----------------------------------------------------------

class StopPlayer2UpCommand : public Command
{
public:
    virtual void execute( DataStruct& rTuple )
    {
        if (rTuple.pBat2->GetCurrentMoveDirection() == eBatMoveDirection::UP)
        rTuple.pBat2->SetDesiredMoveDirection(eBatMoveDirection::NONE);
    }
};

//----------------------------------------------------------

