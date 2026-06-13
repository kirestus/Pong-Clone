#include <headers/dataStruct.h>
#include <SFML/Graphics.hpp>

enum class eCollisionType : std::uint8_t
{
    CollisionWithPlayer1,
    CollisionWithPlayer2,
    CollisionWithWall,
    CollisionWithGoalZone,
    NoCollision
};

