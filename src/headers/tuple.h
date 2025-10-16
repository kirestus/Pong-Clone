#pragma once

#include <headers/bat.h>

struct tuple{
    Bat* pBat1;
    Bat* pBat2;
    sf::Text* pMessage;
    sf::Font* pFont;
    sf::RenderWindow* pRenderWindow;
};