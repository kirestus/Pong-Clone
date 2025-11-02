#pragma once

#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<headers/bat.h>
#include<headers/ball.h>
#include<headers/gameState.h>

//should make this a singleton so that i dont have multiples kicking around

struct DataStruct{
    
    //--GameEntities---
    GameState* pWorldGameState = nullptr;
    Bat* pBat1 = nullptr;
    Bat* pBat2 = nullptr;
    Ball* pBall = nullptr;
    sf::Text* pMessage = nullptr;
    sf::Font* pFont = nullptr;
    sf::RenderWindow* pRenderWindow = nullptr;

    //---SFX----
    sf::SoundBuffer* pPlayer1SoundBuffer = nullptr;
    sf::SoundBuffer* pPlayer2SoundBuffer = nullptr;
    sf::Sound* pPlayer1SoundEffect = nullptr;
    sf::Sound* pPlayer2SoundEffect = nullptr;

    //---GlobalConsts---
    float* fScreenWidth = nullptr;
    float* fScreenHeight = nullptr;
};
