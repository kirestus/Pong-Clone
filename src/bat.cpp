#include <headers/bat.h>

Bat::Bat(sf::Vector2f vPosition)
{
    GetShape().setPosition(vPosition);
    GetShape().setSize(sf::Vector2f(30.0f,170.0f));
    GetShape().setFillColor(sf::Color::White);
}