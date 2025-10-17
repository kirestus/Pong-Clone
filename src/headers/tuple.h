#pragma once

#include<SFML/Graphics.hpp>
#include<headers/bat.h>

struct tuple{
    Bat* pBat1 = nullptr;
    Bat* pBat2 = nullptr;
    sf::Text* pMessage = nullptr;
    sf::Font* pFont = nullptr;
    sf::RenderWindow* pRenderWindow = nullptr;
};

