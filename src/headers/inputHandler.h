#pragma once

#include <SFML/Graphics.hpp>
#include <headers/tuple.h>

class Command
{  
public:
    virtual ~Command() {}
    virtual void execute(tuple& rTuple) = 0;
};


class Player1DownCommand : public Command
{
public:
  virtual void execute(tuple& rTuple)
  {
    rTuple.pBat1->SetDesiredMoveDirection(eBatMoveDirection::DOWN);
  }
};

class Player1UpCommand : public Command
{
public:
  virtual void execute(tuple& rTuple)
  {
    rTuple.pBat1->SetDesiredMoveDirection(eBatMoveDirection::UP);
  }
};


class Player2DownCommand : public Command
{
public:
  virtual void execute(tuple& rTuple)
  {
    rTuple.pBat2->SetDesiredMoveDirection(eBatMoveDirection::DOWN);
  }
};

class Player2UpCommand : public Command
{
public:
  virtual void execute(tuple& rTuple)
  {
    rTuple.pBat2->SetDesiredMoveDirection(eBatMoveDirection::UP);
  }
};


class StartCommand : public Command
{
public:
  virtual void execute(tuple& rTuple)
  {
    rTuple.pBat1->GetCurrentMoveDirection();
  }
};

class StopPlayer1DownCommand : public Command
{
public:
    virtual void execute( tuple& rTuple )
    {
        if (rTuple.pBat1->GetCurrentMoveDirection() == eBatMoveDirection::DOWN)
        rTuple.pBat1->SetDesiredMoveDirection(eBatMoveDirection::NONE);
    }
};

class StopPlayer2DownCommand : public Command
{
public:
    virtual void execute( tuple& rTuple )
    {
        if (rTuple.pBat2->GetCurrentMoveDirection() == eBatMoveDirection::DOWN)
        rTuple.pBat2->SetDesiredMoveDirection(eBatMoveDirection::NONE);
    }
};

class StopPlayer1UpCommand : public Command
{
public:
    virtual void execute( tuple& rTuple )
    {
        if (rTuple.pBat1->GetCurrentMoveDirection() == eBatMoveDirection::UP)
        rTuple.pBat1->SetDesiredMoveDirection(eBatMoveDirection::NONE);
    }
};

class StopPlayer2UpCommand : public Command
{
public:
    virtual void execute( tuple& rTuple )
    {
        if (rTuple.pBat2->GetCurrentMoveDirection() == eBatMoveDirection::UP)
        rTuple.pBat2->SetDesiredMoveDirection(eBatMoveDirection::NONE);
    }
};


class InputHandler
{
public:
    Command* HandleInput( sf::Event* pEvent, Bat* pBat );

private:
    Command* pPlayer1ButtonUp_ = new Player1UpCommand;
    Command* pPlayer1ButtonDown_ = new Player1DownCommand;
    Command* pPlayer2ButtonUp_ = new Player2UpCommand;
    Command* pPlayer2ButtonDown_ = new Player2DownCommand;

    Command* pReleasePlayer1ButtonUp_ = new StopPlayer1UpCommand;
    Command* pReleasePlayer2ButtonUp_ = new StopPlayer2UpCommand;
    Command* pReleasePlayer1ButtonDown_ = new StopPlayer1DownCommand;
    Command* pReleasePlayer2ButtonDown_ = new StopPlayer2DownCommand;

    Command* pStartGame_ = new StartCommand;
};