#pragma once

#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<headers/bat.h>
#include<headers/ball.h>
#include<headers/gameState.h>
#include<memory>
#include<iostream>

//should make this a singleton so that i dont have multiples kicking around

struct DataStruct{
    //--GameEntities---

    std::shared_ptr<sf::RenderWindow> pRenderWindow = std::make_shared<sf::RenderWindow>(sf::VideoMode(1024,768), "PongClone");
    std::shared_ptr<Bat> pBat1 = std::make_shared<Bat>(sf::Vector2f(50.0f,pRenderWindow->getSize().y/2), ePlayerNumber::PLAYER1);
    std::shared_ptr<Bat> pBat2 = std::make_shared<Bat>(sf::Vector2f(pRenderWindow->getSize().x-50.0f, pRenderWindow->getSize().y/2), ePlayerNumber::PLAYER2);
    std::shared_ptr<Ball> pBall = std::make_shared<Ball>(pRenderWindow);

    //---Text----
    std::unique_ptr<GameState> pWorldState = std::make_unique<GameState>();
    std::unique_ptr<sf::Text> pMessage = std::make_unique<sf::Text>();
    std::unique_ptr<sf::Font> pVectorFont = std::make_unique<sf::Font>();

    //---SFX----
    std::unique_ptr<sf::SoundBuffer> pPlayer1SoundBuffer = std::make_unique<sf::SoundBuffer>();
    std::unique_ptr<sf::SoundBuffer> pPlayer2SoundBuffer = std::make_unique<sf::SoundBuffer>();
    std::unique_ptr<sf::SoundBuffer> pHitWallSoundBuffer = std::make_unique<sf::SoundBuffer>();
    std::unique_ptr<sf::SoundBuffer> pScoreGoalSoundBuffer = std::make_unique<sf::SoundBuffer>();
    std::unique_ptr<sf::SoundBuffer> pYouWinSoundBuffer = std::make_unique<sf::SoundBuffer>();

    sf::Sound* pPlayer1SoundEffect = new sf::Sound(*pPlayer1SoundBuffer);
    sf::Sound* pPlayer2SoundEffect = new sf::Sound(*pPlayer2SoundBuffer);
    sf::Sound* pHitWallSoundEffect = new sf::Sound(*pHitWallSoundBuffer);
    sf::Sound* pScoreGoalSoundEffect = new sf::Sound(*pScoreGoalSoundBuffer);
    sf::Sound* pYouWinSoundEffect = new sf::Sound(*pYouWinSoundBuffer);
    sf::Music* pGameMusic = new sf::Music;


    //---GlobalConsts---
    const float fScreenWidth = pRenderWindow->getSize().x;
    const float fScreenHeight = pRenderWindow->getSize().y;

    DataStruct(){
        pGameMusic->openFromFile("sfx/song.mp3");
    }

    ~DataStruct() 
    {
        //need to manually delete these or it will leak
        delete pPlayer1SoundEffect; 
        delete pPlayer2SoundEffect; 
        delete pHitWallSoundEffect;
        delete pScoreGoalSoundEffect;
        delete pYouWinSoundEffect;
        delete pGameMusic;
        std::cout << "DataStruct Destroyed";
    }
};


//Unused singleton keeping it here for use later
/*
class FileSystem
{
public:
    static FileSystem& instance()
    {
        static FileSystem* instance = new FileSystem();
        return *instance;
    }
    

private:
    FileSystem(){}

};
*/