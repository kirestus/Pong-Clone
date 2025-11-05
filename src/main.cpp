#include <SFML/Graphics.hpp>
#include <headers/gameScreen.h>
#include <SFML/System/String.hpp>
#include <headers/bat.h>
#include <headers/dataStruct.h>

int main()
{
    sf::Vector2f vScreenArea(1024, 768);

    sf::RenderWindow window(sf::VideoMode(vScreenArea.x,vScreenArea.y), "PongClone");
    window.requestFocus();
    sf::RenderWindow* ptrWindow = &window;
    
    DataStruct DataTuple; 

    DataTuple.pRenderWindow = &window;
    GameScreen gameScreen = GameScreen(DataTuple);

    return 0;
}
