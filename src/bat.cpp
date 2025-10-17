#include <headers/bat.h>

Bat::Bat(sf::Vector2f vPosition)
{
    m_hRectShape.setPosition(vPosition);
    m_vPosition = vPosition;
    m_hRectShape.setSize(sf::Vector2f(30.0f,170.0f));
    m_hRectShape.setFillColor(sf::Color::White);
    m_hRectShape.setOrigin( sf::Vector2f(m_hRectShape.getSize().x/2, m_hRectShape.getSize().y/2));
}

Bat::Bat()
{
    m_vPosition = sf::Vector2f(70,300);
    m_hRectShape.setSize(sf::Vector2f(30.0f,170.0f));
    m_hRectShape.setFillColor(sf::Color::White);
    m_hRectShape.setOrigin( sf::Vector2f(m_hRectShape.getSize().x/2, m_hRectShape.getSize().y/2));
}

