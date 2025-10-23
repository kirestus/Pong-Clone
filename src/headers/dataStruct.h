#pragma once

#include<SFML/Graphics.hpp>
#include<headers/bat.h>
#include<headers/ball.h>
#include<headers/gameState.h>

//should make this a singleton so that i dont have multiples kicking around

struct DataStruct{
    
    GameState* pWorldGameState = nullptr;
    Bat* pBat1 = nullptr;
    Bat* pBat2 = nullptr;
    Ball* pBall = nullptr;
    sf::Text* pMessage = nullptr;
    sf::Font* pFont = nullptr;
    sf::RenderWindow* pRenderWindow = nullptr;
    
};

