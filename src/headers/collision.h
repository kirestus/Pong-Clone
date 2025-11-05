#include <headers/dataStruct.h>
#include <SFML/Graphics.hpp>

static enum class eCollisionType
{
    CollisionWithPlayer1,
    CollisionWithPlayer2,
    CollisionWithWall,
    CollisionWithGoalZone,
    NoCollision
};

