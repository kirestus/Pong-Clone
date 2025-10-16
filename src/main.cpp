#include <SFML/Graphics.hpp>
#include <headers/bootscreen.h>
#include <SFML/System/String.hpp>
#include <headers/bat.h>
#include <headers/tuple.h>

int main()
{
    tuple DataTuple; 

    sf::RenderWindow window(sf::VideoMode(1280, 720), "PongClone");
    sf::RenderWindow* ptrWindow = &window;
    
    Bat hPlayer1Bat(sf::Vector2f(50.0f,720/2));
    Bat hPlayer2Bat(sf::Vector2f(1280-50.0f,720/2));

    sf::Font fPongFont;
    fPongFont.loadFromFile("fonts/Vector.ttf");

    sf::Text sText("PONG", fPongFont);
    sText.setFillColor(sf::Color::White);
    sText.setOrigin(sText.getLocalBounds().getSize().x/2,sText.getLocalBounds().getSize().y/2);
    sText.setRotation(270.0);
    sText.setPosition(50,720/2); // should set this in the middle of the screen will do later

    DataTuple.pRenderWindow = &window;
    DataTuple.pBat1 = &hPlayer1Bat;
    DataTuple.pBat2 = &hPlayer2Bat;
    DataTuple.pMessage = &sText;
    DataTuple.pFont = &fPongFont;

    BootScreen gameScreen = BootScreen(DataTuple);

    return 0;
}
