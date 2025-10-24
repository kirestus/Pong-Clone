#include <headers/dataStruct.h>

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
    if ( rTuple.pWorldGameState->GetCurrentGameState() == eGameState::Paused )
    {
        rTuple.pWorldGameState->SetDesiredGamestate(eGameState::Running);
    }
    else
    {
        rTuple.pWorldGameState->SetDesiredGamestate(eGameState::Paused);
    }
  }

};

//----------------------------------------------------------

class QuitCommand : public Command
{
public:
  virtual void execute(DataStruct& rTuple)
  {
      rTuple.pRenderWindow->close();
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

