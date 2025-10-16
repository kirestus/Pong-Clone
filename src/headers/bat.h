#pragma once

#include <SFML/Graphics.hpp>

enum class eBatMoveDirection
{
    NONE,
    UP,
    DOWN,
};


class Bat
{
    public:
    Bat(sf::Vector2f vPosition);

    sf::RectangleShape GetShape() const { return m_hRectShape; }
    void SetShape( sf::RectangleShape _shape ) { m_hRectShape = _shape ;}


    private:
        sf::Vector2f m_vPosition;
        sf::RectangleShape m_hRectShape;

        eBatMoveDirection m_eDesiredMoveDirection = eBatMoveDirection::NONE;
        const float m_fYaccel = 0.55f;
        const float m_fYtopSpeed = 5.00f;
        const float m_fYdecaySpeed = 0.9994f;
        float m_fTimeElapsed = 0.00f;
        float m_fVelocity = 0.0f;
};