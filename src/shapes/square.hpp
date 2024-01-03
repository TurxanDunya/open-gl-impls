#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Square
{
public:
    Square(float x, float y, float length);

    enum DIRECTION
    {
        RIGHT =  1,
        LEFT  = -1,
        UP    =  2,
        DOWN  = -2
    };

    glm::vec3 getPosition();
    glm::vec4 getColor();

    void move();
    void setDirection(Square::DIRECTION direction);

private:
    glm::vec4 m_Color;
    glm::vec3 m_Position;
    float     m_Length;

    DIRECTION m_Direction;
};
