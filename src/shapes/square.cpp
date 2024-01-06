#include "square.hpp"

#include <cstdlib>

Square::Square(float x, float y, float length)
    : m_Length(length)
{
    m_Position = glm::vec3(x, y, 0.0f);
    m_Direction = Square::RIGHT;

    m_Color.r = (10 + std::rand() % 15) / 25.0;
    m_Color.g = (10 + std::rand() % 15) / 25.0;
    m_Color.b = (10 + std::rand() % 15) / 25.0;
    m_Color.a = 1.0f;
}

glm::vec3 Square::getPosition() 
{
    return m_Position;
}

glm::vec4 Square::getColor() 
{
    return m_Color;
}

void Square::move()
{
    switch (m_Direction)
    {
    case LEFT:
        m_Position -= glm::vec3(m_Length, 0.0f, 0.0f);
        break;

    case RIGHT:
        m_Position += glm::vec3(m_Length, 0.0f, 0.0f);
        break;
    
    case UP:
        m_Position += glm::vec3(0.0f, m_Length, 0.0f);
        break;

    case DOWN:
        m_Position -= glm::vec3(0.0f, m_Length, 0.0f);
        break;
    }
}

void Square::setDirection(Square::DIRECTION direction)
{
    m_Direction = direction;
}

Square::DIRECTION Square::getDirection() const
{
    return m_Direction;
}