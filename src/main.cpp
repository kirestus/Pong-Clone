#include <SFML/Graphics.hpp>



int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "PongClone");
    sf::Vector2f screenCetre = (sf::Vector2(float(window.getSize().x/2),float(window.getSize().y/2)));
    sf::CircleShape shape(300.f);

    shape.setFillColor(sf::Color::White);
    shape.setOrigin(shape.getRadius(),shape.getRadius());
    shape.setPosition(screenCetre);

    sf::Font vector;
    vector.loadFromFile("./fonts/Vector.ttf");

    sf::Text messageText;
    messageText.setFont(vector);
    messageText.setString("pong");
    messageText.setCharacterSize(30);
    messageText.setFillColor(sf::Color::White);


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