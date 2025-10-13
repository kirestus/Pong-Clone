#include <SFML/Graphics.hpp>

enum class MoveDirection{
    NONE,
    UP,
    DOWN,
};

class BootScreen 
{
    public:

        void DetermineEvent(sf::RenderWindow *pRenderWindow);

        BootScreen(sf::RenderWindow *pRenderWindow);

        sf::CircleShape CreateCircleShape(BootScreen *pBootScreen, float fradius);

        sf::RenderWindow* GetRenderWindow()const {return m_ptr_rWindow;}
        void SetRenderWindow(sf::RenderWindow *pRenderWindow){m_ptr_rWindow = pRenderWindow;}

        sf::Font GetMessageFont()const {return m_ptrVectorFont;}
        void SetMessageFont(sf::Font pFont){m_ptrVectorFont = pFont;}

        void CreateTextBox(BootScreen *pBootScreen);

        sf::Text GetMessageText()const {return m_ptrMessageText;}
        void SetMessageBox(sf::Text pText){m_ptrMessageText = pText;}

        sf::Vector2f CalculateScreenCenter(sf::RenderWindow* pRenderWindow);
        sf::Vector2f GetScreenCenter() const {return m_vScreenCenter;}
        void SetScreenCenter(sf::Vector2f fScreenCenter){m_vScreenCenter = fScreenCenter;}

        sf::CircleShape GetCircleShape() const {return m_sCircleShape;}
        void SetCircleShape(sf::CircleShape pCircle){m_sCircleShape = pCircle;}

        float GetYPos()const {return GetCircleShape().getPosition().y;}

        void CreateBootScreen(BootScreen *pBootScreen);
        int UpdateBootscreen(BootScreen *pBootScreen, sf::Clock &rTimer);

        sf::Clock GetGameClock() const {return m_hClock;}


    private:         
        sf::RenderWindow* m_ptr_rWindow;
        sf::Text m_ptrMessageText;
        sf::Font m_ptrVectorFont;
        sf::Vector2f m_vScreenCenter;
        sf::CircleShape m_sCircleShape;
        sf::Clock m_hClock;

        sf::Vector2f m_vPosition;

        MoveDirection m_eDesiredMoveDirection = MoveDirection::NONE;
        const float m_fYaccel = 0.55f;
        const float m_fYtopSpeed = 5.00f;
        const float m_fYdecaySpeed = 0.9994f;
        float m_fTimeElapsed = 0.00f;
        float m_fVelocity = 0.0f;

        //bool m_bIsTopspeed = (m_fVelocity > m_fYtopSpeed || m_fVelocity < -m_fYtopSpeed) ; 
};

