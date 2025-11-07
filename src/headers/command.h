#include <headers/dataStruct.h>
#include <headers/debugText.h>

class Command
{  
public:
    virtual ~Command() {}
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

class RestartGameCommand : public Command
{
public:
  virtual void execute(DataStruct& rTuple)
  {
    // need to make the gamestate gatherable
    //void GameScreen::ResetGame(DataStruct &rTuple)
  }
};

//----------------------------------------------------------



class DebugTextGameStateCommand : public Command
{
public:
  virtual void execute(DataStruct& rTuple)
  {
    rTuple.pMessage->setString(DebugText::DebugTextGameState(rTuple.pWorldState->GetCurrentGameState()));
  }
};

//----------------------------------------------------------

class DebugTextBallStateCommand : public Command
{
public:
  virtual void execute(DataStruct& rTuple)
  {
    rTuple.pMessage->setString(DebugText::DebugTextBallState(rTuple.pBall->GetCurrentBallState()));
  }
};

//----------------------------------------------------------

class PauseMusicCommand : public Command
{
public:
  virtual void execute(DataStruct& rTuple)
  {
    if (!rTuple.pGameMusic->Paused)
    {
      rTuple.pGameMusic->pause();
    }
    else
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
      rTuple.~DataStruct();
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

