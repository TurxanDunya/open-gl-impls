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

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "shaderprogram/program.hpp"
#include "shapes/square.hpp"

struct Vertex
{
    glm::vec3 position;
    glm::vec2 texture;
};

float rotationAngle;
glm::vec2 position;
float scale;

std::vector<Vertex> vertices;
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

        // vertices.push_back(glm::vec3(x, y, z));
    }

    for (int i = 0; i < triangleCount; i++)
    {
        indices.push_back(0);
        indices.push_back(i + 1);
        indices.push_back(i + 2);
    }
}

void createSquare(float length)
{
    Vertex v0, v1, v2, v3;
    v0.position = glm::vec3(-length/2, length/2, 0.0f);
    v1.position = glm::vec3(-length/2, -length/2, 0.0f);
    v2.position = glm::vec3(length/2, -length/2, 0.0f);
    v3.position = glm::vec3(length/2, length/2, 0.0f);

    v0.texture = glm::vec2(0.0f, 1.0f);
    v1.texture = glm::vec2(0.0f, 0.0f);
    v2.texture = glm::vec2(1.0f, 0.0f);
    v3.texture = glm::vec2(1.0f, 1.0f);

    vertices.push_back(v0);
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(3);
    
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(3);
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

    //createCircle(0.2f, 12);
    createSquare(1.0f);
    
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // ------ CREATE VERTEX ATTRIB POINTER OBJECT END ------

    // ------ LOAD TEXTURE ------
    int width, height, nrChannels;
    unsigned char* imageData = stbi_load("../images/texture_1.png", &width, &height, &nrChannels, 0);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(imageData);
    // ------ LOAD TEXTURE END ------

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

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glBindVertexArray(VAO);

        program.setVec4ValueToUniform("uColor", glm::vec4(0.4f, 0.8f, 0.5f, 1.0f));
        program.setMat3ValueToUniform("uMtxTransform", &mtxTransform);
        
        // glDrawArray is using vertex buffer directly
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        std::this_thread::sleep_for (std::chrono::milliseconds(100));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
        
}
