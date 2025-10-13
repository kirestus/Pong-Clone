#include <SFML/Graphics.hpp>
class Bat
{
    public:


    private:
        sf::Vector2f m_vPosition;
        sf::RectangleShape m_hRectShape;

        
        const float m_fYaccel = 0.055f;
        const float m_fYtopSpeed = 1.0f;
        const float m_fYdecaySpeed = 0.001f;
        float m_fTimeElapsed = 0.00f;
        bool m_isGoingUp = false;
        bool m_bIsTopspeed = (m_vPosition.y > m_fYtopSpeed || m_vPosition.y < -m_fYtopSpeed) ; 
};