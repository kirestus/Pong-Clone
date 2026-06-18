#include <headers/dataStruct.h>
#include <SFML/Graphics.hpp>
#include <headers/alias.h>

enum class eCollisionType : uint8
{
    CollisionWithPlayer1,
    CollisionWithPlayer2,
    CollisionWithWall,
    CollisionWithGoalZone,
    NoCollision
};

