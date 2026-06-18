#pragma once
#include <headers/alias.h>

enum class eBallState : uint8
{
    None,
    LEFT,
    RIGHT,
    HitBall,
    HitTopWall,
    HitBottomWall,
    AtPlayer1,
    AtPlayer2,
    GoalOnPlayer1,
    GoalOnPlayer2,
    ResetGamePosition,
};