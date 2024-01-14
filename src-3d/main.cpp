#include <iostream>
#include <vector>
#include <thread>
#include <chrono> 

#include <glad/glad.h>
#include <GlFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shaderprogram/program.hpp"
#include "shapes/square.hpp"
#include "texture/texturemanager.hpp"

struct Vertex
{
    glm::vec3 position;
    glm::vec4 color;
};

float rotationAngle;
glm::vec3 position;
float scale;

std::vector<Vertex> vertices;
std::vector<unsigned int> indices;

void createSquare(float length)
{
    Vertex v0, v1, v2, v3, v4, v5, v6, v7;
    v0.position = glm::vec3(-length/2, -length/2, -length/2);
    v1.position = glm::vec3( length/2, -length/2, -length/2);
    v2.position = glm::vec3( length/2, -length/2,  length/2);
    v3.position = glm::vec3(-length/2, -length/2,  length/2);
    v4.position = glm::vec3(-length/2,  length/2, -length/2);
    v5.position = glm::vec3( length/2,  length/2, -length/2);
    v6.position = glm::vec3( length/2,  length/2,  length/2);
    v7.position = glm::vec3(-length/2,  length/2,  length/2);

    v0.color = glm::vec4(1.0f,0.0f,0.0f,1.0f);
    v1.color = glm::vec4(1.0f,1.0f,0.0f,1.0f);
    v2.color = glm::vec4(1.0f,0.0f,1.0f,1.0f);
    v3.color = glm::vec4(0.0f,1.0f,0.0f,1.0f);
    v4.color = glm::vec4(1.0f,0.0f,1.0f,1.0f);
    v5.color = glm::vec4(0.0f,1.0f,1.0f,1.0f);
    v6.color = glm::vec4(0.0f,0.0f,1.0f,1.0f);
    v7.color = glm::vec4(1.0f,1.0f,0.0f,1.0f);

    vertices.push_back(v0);
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);
    vertices.push_back(v5);
    vertices.push_back(v6);
    vertices.push_back(v7);

    // front face
    indices.push_back(7);
    indices.push_back(3);
    indices.push_back(2);
    
    indices.push_back(7);
    indices.push_back(2);
    indices.push_back(6);

    // right face
    indices.push_back(6);
    indices.push_back(2);
    indices.push_back(1);
    
    indices.push_back(6);
    indices.push_back(1);
    indices.push_back(5);

    // face 3
    indices.push_back(4);
    indices.push_back(7);
    indices.push_back(6);
    
    indices.push_back(4);
    indices.push_back(6);
    indices.push_back(5);

    // bottom face
    indices.push_back(0);
    indices.push_back(3);
    indices.push_back(2);
    
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(1);

    // face 5
    indices.push_back(4);
    indices.push_back(0);
    indices.push_back(1);
    
    indices.push_back(4);
    indices.push_back(1);
    indices.push_back(5);

    // face 6
    indices.push_back(7);
    indices.push_back(3);
    indices.push_back(0);
    
    indices.push_back(7);
    indices.push_back(0);
    indices.push_back(4);
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
    program.attachShader("../src-3d/shaders/vertexshader.glsl", GL_VERTEX_SHADER);
    program.attachShader("../src-3d/shaders/fragmentshader.glsl", GL_FRAGMENT_SHADER);
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // ------ CREATE VERTEX ATTRIB POINTER OBJECT END ------

    // ------ LOAD TEXTURE ------
    unsigned int textureId = TextureManager::getInstance() -> loadTexture("texture_1.png");
    // ------ LOAD TEXTURE END ------

    rotationAngle = 0.0f;
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = 1.0f;

    glm::mat4 mtxTransform(1);
    glm::mat4 mtxProjection = glm::perspective(
        glm::radians(90.0f), (800.0f / 800.0f), 1.0f, 100.0f);

    glm::vec3 cameraPosition(0.0f, 2.0f, 5.0f);
    glm::vec3 cameraLookAt(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
    glm::mat4 mtxCamera = glm::lookAt(cameraPosition, cameraLookAt, cameraUp);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.4f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 mtxRotation = glm::rotate(glm::mat4(1), glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        mtxTransform = mtxProjection * mtxCamera * mtxRotation;

        rotationAngle += 1.0f;

        program.use();

        TextureManager::getInstance() -> activateTexture(GL_TEXTURE0, textureId);

        glBindVertexArray(VAO);

        program.setVec4ValueToUniform("uColor", glm::vec4(0.4f, 0.8f, 0.5f, 1.0f));
        program.setMat4ValueToUniform("uMtxTransform", &mtxTransform);
        
        // glDrawArray is using vertex buffer directly
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        std::this_thread::sleep_for (std::chrono::milliseconds(100));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
        
}
