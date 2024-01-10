#include <iostream>
#include <vector>
#include <thread>
#include <chrono> 

#include <glad/glad.h>
#include <GlFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

#include "shaderprogram/program.hpp"
#include "shapes/square.hpp"

float rotationAngle;
glm::vec2 position;
float scale;

std::vector<glm::vec3> vertices;
std::vector<unsigned int> indices;

void createCircle(float radius, int vertexCount)
{
    float angle = 360.0f / vertexCount;
    int triangleCount = vertexCount - 2;

    for (int i = 0; i < vertexCount; i++)
    {
        float newAngle = angle * i;

        float x = radius * cos(glm::radians(newAngle));
        float y = radius * sin(glm::radians(newAngle));
        float z = 1.0f;

        vertices.push_back(glm::vec3(x, y, z));
    }

    for (int i = 0; i < triangleCount; i++)
    {
        indices.push_back(0);
        indices.push_back(i + 1);
        indices.push_back(i + 2);
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

    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        position.x -= 0.01f;

    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        position.x += 0.01f;

    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        position.y += 0.01f;
    
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        position.y -= 0.01f;

    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        scale -= 0.1f;

    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
        scale += 0.1f;
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

    createCircle(0.2f, 12);
    
    // ------ CREATE SHADER PROGRAM ------
    Program program;
    program.attachShader("../shaders/vertexshader.glsl", GL_VERTEX_SHADER);
    program.attachShader("../shaders/fragmentshader.glsl", GL_FRAGMENT_SHADER);
    program.link();
    // ------ CREATE SHADER PROGRAM END ------

    program.addUniform("uColor");
    program.addUniform("uMtxTransform");

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

    // ------ CREATE INDEX BUFFER OBJECT ------
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
    // ------ CREATE INDEX BUFFER OBJECT END ------

    // ------ CREATE VERTEX ATTRIB POINTER OBJECT END ------
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);
    // ------ CREATE VERTEX ATTRIB POINTER OBJECT END ------

    rotationAngle = 0.0f;
    position = glm::vec2(0.0f, 0.0f);
    scale = 1.0f;
    glm::mat3 mtxTransform(1);
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.4f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat3 mtxTranslation = glm::translate(glm::mat3(1), position);
        glm::mat3 mtxRotation = glm::rotate(glm::mat3(1), glm::radians(rotationAngle));
        glm::mat3 mtxScale = glm::scale(glm::mat3(1), glm::vec2(scale, scale));
        mtxTransform = mtxTranslation * mtxRotation * mtxScale; // T x R x S x vertex

        rotationAngle += 1.0f;

        program.use();
        glBindVertexArray(VAO);
        glEnableVertexAttribArray(0);

        program.setVec4ValueToUniform("uColor", glm::vec4(0.4f, 0.8f, 0.5f, 1.0f));
        program.setMat3ValueToUniform("uMtxTransform", &mtxTransform);
        // glDrawArray is using vertex buffer directly
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        std::this_thread::sleep_for (std::chrono::milliseconds(100));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
        
}
