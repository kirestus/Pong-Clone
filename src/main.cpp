#include <SFML/Graphics.hpp>
#include <headers/gameScreen.h>
#include <SFML/System/String.hpp>
#include <headers/bat.h>
#include <headers/dataStruct.h>

int main()
{
    DataStruct DataTuple; 
    sf::Vector2f vScreenArea(1024, 768);

    sf::RenderWindow window(sf::VideoMode(vScreenArea.x,vScreenArea.y), "PongClone");
    sf::RenderWindow* ptrWindow = &window;
    
    Bat Player1Bat(sf::Vector2f(50.0f,vScreenArea.y/2), ePlayerNumber::PLAYER1);
    Bat Player2Bat(sf::Vector2f(vScreenArea.x-50.0f,vScreenArea.y/2), ePlayerNumber::PLAYER2);
    Ball Ball(ptrWindow);

    sf::Font fPongFont;
    fPongFont.loadFromFile("fonts/Vector.ttf");

    sf::Text sText("", fPongFont);
    sText.setFillColor(sf::Color::White);
    sText.setCharacterSize(55);
    sText.setOrigin(sText.getLocalBounds().getSize().x/2,sText.getLocalBounds().getSize().y/2);
    sText.setPosition(sf::Vector2f(vScreenArea.x/2-150,vScreenArea.y+-100.0f) ); // should set this in the middle of the screen will do later

    DataTuple.pRenderWindow = &window;
    DataTuple.pBat1 = &Player1Bat;
    DataTuple.pBat2 = &Player2Bat;
    DataTuple.pBall = &Ball;
    DataTuple.pMessage = &sText;
    DataTuple.pFont = &fPongFont;


    GameScreen gameScreen = GameScreen(DataTuple);

    return 0;
}
