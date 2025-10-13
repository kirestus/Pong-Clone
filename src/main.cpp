#include <SFML/Graphics.hpp>
#include <headers/bootscreen.h>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "PongClone");
    sf::RenderWindow* ptrWindow = &window;
    
    BootScreen gameScreen = BootScreen(ptrWindow);

    return 0;
}