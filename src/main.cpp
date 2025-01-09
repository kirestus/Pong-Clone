#include <SFML/Graphics.hpp>



int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "PongClone");
    sf::Vector2f screenCetre = sf::Vector2f(window.getSize().x/2, window.getSize().y/2);
    sf::CircleShape shape(300.f);

    shape.setFillColor(sf::Color::Blue);
    shape.setOrigin(shape.getRadius(),shape.getRadius());
    shape.setPosition(screenCetre);

    sf::Font vector;
    vector.loadFromFile("/Users/johnfry/SFML_Projects/Pong-Clone/src/fonts/Vector.ttf");

    sf::Text messageText;
    messageText.setFont(vector);
    messageText.setString("PonG");
    messageText.setCharacterSize(56);
    messageText.setFillColor(sf::Color::Green);
    messageText.setPosition(screenCetre.x,screenCetre.y);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.draw(messageText);
        window.display();
    }

    return 0;
}