#include <SFML/Graphics.hpp>
#include <headers/dataStruct.h>

class CollisionSystem
{
public:

float CreateRandomAngle(const int16 maxRange);
void HandleCollisions(const DataStruct &rTuple, const eCollisionType eCollidingwith,const int64 iSimFrame );
eCollisionType CheckCollisions(const DataStruct &rTuple);
bool isBallCollidingWithTarget(const sf::FloatRect box1, const sf::FloatRect box2);
bool isBallHittingWall(const sf::FloatRect box1, const std::shared_ptr<sf::RenderWindow> pRenderWindow);
bool isBallHittingGoal(const sf::FloatRect box1,const DataStruct &rTuple/*,const bool isPaused*/ );
};