#pragma once

enum class eBallState : uint8_t{
    None,
    LEFT,
    RIGHT,
    HitBall,
    HitWall,
    AtPlayer1,
    AtPlayer2,
    GoalOnPlayer1,
    GoalOnPlayer2,
    ResetGamePosition,
};