#include <headers/dataStruct.h>
#include <SFML/Graphics.hpp>

class cogCollider
{
public:
    cogCollider();
    virtual ~cogCollider();

    sf::RectangleShape* GetObjectsCollisionRect() const { return m_pColliderRect ; }

private:
    sf::RectangleShape* m_pColliderRect;
    bool m_bIsColliding = false;
};




class CollisionHandler
{

public:



private:

};