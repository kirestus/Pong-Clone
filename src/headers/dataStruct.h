#pragma once

#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<headers/bat.h>
#include<headers/ball.h>
#include<headers/gameState.h>

//should make this a singleton so that i dont have multiples kicking around

struct DataStruct{
    
    //--GameEntities---

    sf::RenderWindow* pRenderWindow = nullptr;
    GameState* pWorldGameState = nullptr;
    Bat* pBat1 = nullptr;
    Bat* pBat2 = nullptr;
    Ball* pBall = nullptr;
    sf::Text* pMessage = new sf::Text;
    sf::Font vectorFont;
    

    //---SFX----
    sf::SoundBuffer* pPlayer1SoundBuffer = new sf::SoundBuffer;
    sf::SoundBuffer* pPlayer2SoundBuffer = new sf::SoundBuffer;
    sf::SoundBuffer* pHitWallSoundBuffer = new sf::SoundBuffer;

    sf::Sound* pPlayer1SoundEffect = new sf::Sound(*pPlayer1SoundBuffer);
    sf::Sound* pPlayer2SoundEffect = new sf::Sound(*pPlayer2SoundBuffer);
    sf::Sound* pHitWallSoundEffect = new sf::Sound(*pHitWallSoundBuffer);

    //---GlobalConsts---
    float fScreenWidth;
    float fScreenHeight;
};
