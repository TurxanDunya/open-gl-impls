#include <iostream>
#include <vector>
#include <thread>
#include <chrono> 

#include <glad/glad.h>
#include <GlFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>

#include "shaderprogram/program.hpp"
#include "shapes/square.hpp"

std::vector<glm::vec3> vertices;

void createCircle(float radius, int vertexCount)
{
    float angle = 360.0f / vertexCount;
    int triangleCount = vertexCount - 2;

    std::vector<glm::vec3> tempVertices;
    for (int i = 0; i < vertexCount; i++)
    {
        float newAngle = angle * i;

        float x = radius * cos(glm::radians(newAngle));
        float y = radius * sin(glm::radians(newAngle));
        float z = 0.0f;

        tempVertices.push_back(glm::vec3(x, y, z));
    }

    for (int i = 0; i < triangleCount; i++)
    {
        vertices.push_back(tempVertices[0]);
        vertices.push_back(tempVertices[i + 1]);
        vertices.push_back(tempVertices[i + 2]);
    }
}

void errorCallback(int error, const char* description) 
{
    std::cerr << "GLFW error (" << error << "): " << description << std::endl;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
    if (key == GLFW_KEY_ESCAPE) 
        glfwTerminate();

    Square* firstPart = new Square(0.0f, 0.0f, 1.0f);
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
    
    GLFWwindow* window = glfwCreateWindow(800, 800, "Hello OPENGL", NULL, NULL);

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

    createCircle(1, 12);
    
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
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

        program.setVec3ValueToUniform("uMove", glm::vec3(0.0f, 0.0f, 0.0f));
        program.setVec4ValueToUniform("uColor", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        std::this_thread::sleep_for (std::chrono::milliseconds(100));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
        
}
