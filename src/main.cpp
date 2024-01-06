#include <iostream>
#include <vector>
#include <thread>
#include <chrono> 

#include <glad/glad.h>
#include <GlFW/glfw3.h>
#include <glm/vec3.hpp>

#include "shaderprogram/program.hpp"
#include "shapes/square.hpp"

void addToSnakeTail();
void drawSnake(Program&);
void moveSnake();

std::vector<Square*> snakeParts;

float length = 0.1f;
float vertices[] = {
    -length / 2, -length / 2, 0.0f,
     length / 2, -length / 2, 0.0f,
     length / 2,  length / 2, 0.0f,

    -length / 2,  length / 2, 0.0f,
    -length / 2, -length / 2, 0.0f,
     length / 2,  length / 2, 0.0f
};

void errorCallback(int error, const char* description) 
{
    std::cerr << "GLFW error (" << error << "): " << description << std::endl;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
    if (key == GLFW_KEY_ESCAPE) 
        glfwTerminate();

    if (key == GLFW_KEY_SPACE)
    {
        addToSnakeTail();
    }
    
    Square* firstPart = snakeParts[0];
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        firstPart -> setDirection(Square::LEFT);

    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        firstPart -> setDirection(Square::RIGHT);

    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        firstPart -> setDirection(Square::UP);
    
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        firstPart -> setDirection(Square::DOWN);
}

int main(int argc, char** argv)
{
    if (!glfwInit())
    {
       std::cout << "Error in initializing GLFW!" << std::endl; 
       return -1;
    }

    glfwSetErrorCallback(errorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For Mac only
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello OPENGL", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Error in creating GLFW window!" << std::endl; 
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD!" << std::endl; 
        return -1;
    }

    for (int i = 0; i < 4; i++)
    {
        addToSnakeTail();
    }
    
    // ------ CREATE SHADER PROGRAM ------
    Program program;
    program.attachShader("../shaders/vertexshader.glsl", GL_VERTEX_SHADER);
    program.attachShader("../shaders/fragmentshader.glsl", GL_FRAGMENT_SHADER);
    program.link();
    // ------ CREATE SHADER PROGRAM END ------

    program.addUniform("uMove");
    program.addUniform("uColor");

    // ------ CREATE VERTEX BUFFER OBJECT ------
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // ------ CREATE VERTEX BUFFER OBJECT END ------

    // ------ CREATE VERTEX ARRAY OBJECT ------
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // ------ CREATE VERTEX BUFFER OBJECT END ------

    // ------ CREATE VERTEX ATTRIB POINTER OBJECT END ------
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);
    // ------ CREATE VERTEX ATTRIB POINTER OBJECT END ------

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.4f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        program.use();
        glBindVertexArray(VAO);
        glEnableVertexAttribArray(0);

        drawSnake(program);
        moveSnake();

        std::this_thread::sleep_for (std::chrono::milliseconds(100));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
        
}

void drawSnake(Program& program)
{
    for (auto snakePart: snakeParts)
    {
        program.setVec3ValueToUniform("uMove", snakePart -> getPosition());
        program.setVec4ValueToUniform("uColor", snakePart -> getColor());
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

void addToSnakeTail() 
{
    int elementCount = snakeParts.size();
    if (elementCount == 0)
    {
        snakeParts.push_back(new Square(0.0f, 0.0f, length));
    }
    else
    {
        Square* lastSquare = snakeParts[elementCount - 1];

        glm::vec3 position = lastSquare -> getPosition();
        Square::DIRECTION direction = lastSquare -> getDirection();

        switch (direction)
        {
            case Square::LEFT:
                position -= glm::vec3(length, 0.0f, 0.0f);
                break;

            case Square::RIGHT:
                position += glm::vec3(length, 0.0f, 0.0f);
                break;
            
            case Square::UP:
                position += glm::vec3(0.0f, length, 0.0f);
                break;

            case Square::DOWN:
                position -= glm::vec3(0.0f, length, 0.0f);
                break;
        }

        Square* newSquare = new Square(position.x, position.y, length);
        newSquare -> setDirection(direction);
        snakeParts.push_back(newSquare);
    }
}

void moveSnake()
{
    for (auto snakePart: snakeParts)
    {
        snakePart -> move();
    }

    for(int i = snakeParts.size() - 1; i > 0; i--)
    {
        snakeParts[i] -> setDirection(snakeParts[i - 1] -> getDirection());
    }
}