#pragma once

#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<headers/bat.h>
#include<headers/ball.h>
#include<headers/gameState.h>
#include<headers/debugText.h>
#include<memory>


struct DataStruct{
    //--GameEntities---

    private:
        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
        unsigned int width = desktop.width;
        unsigned int height = desktop.height;

        const sf::Vector2f baseResolution {static_cast<float>(width),static_cast<float>(height)};
        const sf::Vector2f screenCenter{ baseResolution.x/2 , baseResolution.y/2 };

    public:
    static constexpr float g_fBAT_OFFSET = 100.0f;

    std::shared_ptr<sf::RenderWindow> pRenderWindow = std::make_shared<sf::RenderWindow>(sf::VideoMode::getDesktopMode(), "Turbo Pong", sf::Style::Fullscreen);
    
    std::shared_ptr<sf::View> pView = std::make_shared<sf::View>(sf::View(screenCenter, baseResolution));

    std::shared_ptr<Bat> pBat1 = std::make_shared<Bat>(sf::Vector2f(g_fBAT_OFFSET,pView->getSize().y/2), ePlayerNumber::PLAYER1);
    std::shared_ptr<Bat> pBat2 = std::make_shared<Bat>(sf::Vector2f(pView->getSize().x-g_fBAT_OFFSET, pRenderWindow->getSize().y/2), ePlayerNumber::PLAYER2);
    std::shared_ptr<Ball> pBall = std::make_shared<Ball>(pRenderWindow);

    //---Text----
    //std::unique_ptr<GameState> pWorldState = std::make_unique<GameState>();
    std::unique_ptr<cogGameState> pCogGameState = std::make_unique<cogGameState>();
    std::unique_ptr<DesiredGameState> pDesiredWorldState = std::make_unique<DesiredGameState>();
    std::unique_ptr<CurrentGameState> pCurrentWorldState = std::make_unique<CurrentGameState>();

    std::unique_ptr<DebugText> pDebugText = std::make_unique<DebugText>();
    std::unique_ptr<sf::Text> pMessageText = std::make_unique<sf::Text>();
    std::unique_ptr<sf::Text> pScoreText = std::make_unique<sf::Text>();
    std::unique_ptr<sf::Font> pVectorFont = std::make_unique<sf::Font>();


    //---VFX----

    //---SFX----
    std::unique_ptr<sf::SoundBuffer> pPlayer1SoundBuffer = std::make_unique<sf::SoundBuffer>();
    std::unique_ptr<sf::SoundBuffer> pPlayer2SoundBuffer = std::make_unique<sf::SoundBuffer>();
    std::unique_ptr<sf::SoundBuffer> pHitWallSoundBuffer = std::make_unique<sf::SoundBuffer>();
    std::unique_ptr<sf::SoundBuffer> pScoreGoalSoundBuffer = std::make_unique<sf::SoundBuffer>();
    std::unique_ptr<sf::SoundBuffer> pSpitBallSoundBuffer = std::make_unique<sf::SoundBuffer>();
    std::unique_ptr<sf::SoundBuffer> pYouWinSoundBuffer = std::make_unique<sf::SoundBuffer>();

    sf::Sound* pPlayer1SoundEffect = new sf::Sound(*pPlayer1SoundBuffer);
    sf::Sound* pPlayer2SoundEffect = new sf::Sound(*pPlayer2SoundBuffer);
    sf::Sound* pHitWallSoundEffect = new sf::Sound(*pHitWallSoundBuffer);
    sf::Sound* pScoreGoalSoundEffect = new sf::Sound(*pScoreGoalSoundBuffer);
    sf::Sound* pSpitBallSoundEffect = new sf::Sound(*pSpitBallSoundBuffer);
    sf::Sound* pYouWinSoundEffect = new sf::Sound(*pYouWinSoundBuffer);
    sf::Music* pGameMusic = new sf::Music;

    //---GlobalConsts---
    const float fScreenWidth = pView->getSize().x;
    const float fScreenHeight = pView->getSize().y;

    DataStruct(){
        pGameMusic->openFromFile("sfx/song.mp3");
        //const auto fullscreenModes = sf::VideoMode::getFullscreenModes();
        //pRenderWindow = std::make_shared<sf::RenderWindow>(fullscreenModes[0], "Turbo Pong", sf::Style::Fullscreen);

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
    }
};

