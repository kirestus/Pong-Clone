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
    window.requestFocus();
    sf::RenderWindow* ptrWindow = &window;
    
    Bat Player1Bat(sf::Vector2f(50.0f,ptrWindow->getSize().y/2), ePlayerNumber::PLAYER1);
    Bat Player2Bat(sf::Vector2f(vScreenArea.x-50.0f,ptrWindow->getSize().y/2), ePlayerNumber::PLAYER2);
    Ball Ball(ptrWindow);

    sf::Font fPongFont;
    fPongFont.loadFromFile("fonts/Vector.ttf");

    sf::Text sText("PONG", fPongFont);
    sText.setFillColor(sf::Color::White);
    sText.setCharacterSize(90);
    sText.setOrigin(sText.getLocalBounds().getSize().x/2,sText.getLocalBounds().getSize().y/2);
    sText.setPosition(sf::Vector2f(vScreenArea.x/2,vScreenArea.y+-100.0f) ); // should set this in the middle of the screen will do later

    sf::SoundBuffer player1SoundBuffer;
    sf::SoundBuffer player2SoundBuffer;

    player1SoundBuffer.loadFromFile("sfx/player1Hit.wav");
    sf::Sound p1SoundEffect;
    p1SoundEffect.setBuffer(player1SoundBuffer);

    player2SoundBuffer.loadFromFile("sfx/player2Hit.wav");
    sf::Sound p2SoundEffect;
    p2SoundEffect.setBuffer(player2SoundBuffer);

    DataTuple.pRenderWindow = &window;
    DataTuple.fScreenWidth = &vScreenArea.x;
    DataTuple.fScreenHeight = &vScreenArea.y;
    DataTuple.pBat1 = &Player1Bat;
    DataTuple.pBat2 = &Player2Bat;
    DataTuple.pBall = &Ball;
    DataTuple.pMessage = &sText;
    DataTuple.pFont = &fPongFont;
    DataTuple.pPlayer1SoundBuffer = &player1SoundBuffer;
    DataTuple.pPlayer1SoundEffect = &p1SoundEffect;
    DataTuple.pPlayer2SoundBuffer = &player2SoundBuffer;
    DataTuple.pPlayer2SoundEffect = &p2SoundEffect;

    GameScreen gameScreen = GameScreen(DataTuple);

    return 0;
}
