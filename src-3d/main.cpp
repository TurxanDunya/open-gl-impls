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
    glm::vec2 texCoord;
};

float rotationAngle;
glm::vec3 position;
float scale;

std::vector<Vertex> vertices;
std::vector<unsigned int> indices;

void createSquare(float length)
{
    glm::vec3 v[8];
    v[0] = glm::vec3(-length / 2, -length / 2, -length / 2);
    v[1] = glm::vec3( length / 2, -length / 2, -length / 2);
    v[2] = glm::vec3( length / 2, -length / 2,  length / 2);
    v[3] = glm::vec3(-length / 2, -length / 2,  length / 2);
    v[4] = glm::vec3(-length / 2,  length / 2, -length / 2);
    v[5] = glm::vec3( length / 2,  length / 2, -length / 2);
    v[6] = glm::vec3( length / 2,  length / 2,  length / 2);
    v[7] = glm::vec3(-length / 2,  length / 2,  length / 2);

    Vertex tempVertices[24];

    // face 1
    tempVertices[0].position = v[7];
    tempVertices[1].position = v[3];
    tempVertices[2].position = v[2];
    tempVertices[3].position = v[6];
    tempVertices[0].texCoord = glm::vec2(0.0f, 1.0f);
    tempVertices[1].texCoord = glm::vec2(0.0f, 0.0f);
    tempVertices[2].texCoord = glm::vec2(1.0f, 0.0f);
    tempVertices[3].texCoord = glm::vec2(1.0f, 1.0f);

    // face 2
    tempVertices[4].position = v[6];
    tempVertices[5].position = v[2];
    tempVertices[6].position = v[1];
    tempVertices[7].position = v[5];
    tempVertices[4].texCoord = glm::vec2(0.0f, 1.0f);
    tempVertices[5].texCoord = glm::vec2(0.0f, 0.0f);
    tempVertices[6].texCoord = glm::vec2(1.0f, 0.0f);
    tempVertices[7].texCoord = glm::vec2(1.0f, 1.0f);

    // face 3
    tempVertices[8 ].position = v[4];
    tempVertices[9 ].position = v[7];
    tempVertices[10].position = v[6];
    tempVertices[11].position = v[5];
    tempVertices[8 ].texCoord = glm::vec2(0.0f, 1.0f);
    tempVertices[9 ].texCoord = glm::vec2(0.0f, 0.0f);
    tempVertices[10].texCoord = glm::vec2(1.0f, 0.0f);
    tempVertices[11].texCoord = glm::vec2(1.0f, 1.0f);

    // face 4
    tempVertices[12].position = v[0];
    tempVertices[13].position = v[3];
    tempVertices[14].position = v[2];
    tempVertices[15].position = v[1];
    tempVertices[12].texCoord = glm::vec2(0.0f, 1.0f);
    tempVertices[13].texCoord = glm::vec2(0.0f, 0.0f);
    tempVertices[14].texCoord = glm::vec2(1.0f, 0.0f);
    tempVertices[15].texCoord = glm::vec2(1.0f, 1.0f);

    // face 5
    tempVertices[16].position = v[4];
    tempVertices[17].position = v[0];
    tempVertices[18].position = v[1];
    tempVertices[19].position = v[5];
    tempVertices[16].texCoord = glm::vec2(0.0f, 1.0f);
    tempVertices[17].texCoord = glm::vec2(0.0f, 0.0f);
    tempVertices[18].texCoord = glm::vec2(1.0f, 0.0f);
    tempVertices[19].texCoord = glm::vec2(1.0f, 1.0f);

    // face 6
    tempVertices[20].position = v[7];
    tempVertices[21].position = v[3];
    tempVertices[22].position = v[0];
    tempVertices[23].position = v[4];
    tempVertices[20].texCoord = glm::vec2(0.0f, 1.0f);
    tempVertices[21].texCoord = glm::vec2(0.0f, 0.0f);
    tempVertices[22].texCoord = glm::vec2(1.0f, 0.0f);
    tempVertices[23].texCoord = glm::vec2(1.0f, 1.0f);

    for (int i = 0; i < 24; i++)
    {
        vertices.push_back(tempVertices[i]);
    }

    for (int i = 0; i < 6; i++)
    {
        int startIndex = 4 * i;

        indices.push_back(startIndex);
        indices.push_back(startIndex + 1);
        indices.push_back(startIndex + 2);

        indices.push_back(startIndex);
        indices.push_back(startIndex + 2);
        indices.push_back(startIndex + 3);
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
        position.x -= 0.1f;

    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        position.x += 0.1f;

    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        position.y += 0.1f;
    
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        position.y -= 0.1f;

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

    createSquare(1.0f);
    
    // ------ CREATE SHADER PROGRAM ------
    Program program;
    program.attachShader("../src-3d/shaders/vertexshader.glsl", GL_VERTEX_SHADER);
    program.attachShader("../src-3d/shaders/fragmentshader.glsl", GL_FRAGMENT_SHADER);
    program.link();
    // ------ CREATE SHADER PROGRAM END ------

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
    unsigned int textureId = TextureManager::getInstance() -> loadTexture("texture_2.jpg");
    // ------ LOAD TEXTURE END ------

    rotationAngle = 0.0f;
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = 1.0f;

    glm::mat4 mtxTransform(1);
    glm::mat4 mtxProjection = glm::perspective(
        glm::radians(90.0f), (800.0f / 800.0f), 1.0f, 100.0f);

    glm::vec3 cameraPosition(2.0f, 2.0f, 2.0f);
    glm::vec3 cameraLookAt(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
    glm::mat4 mtxCamera = glm::lookAt(cameraPosition, cameraLookAt, cameraUp);
 
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.4f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 mtxTranslation = glm::translate(glm::mat4(1), position);
        glm::mat4 mtxRotation = glm::rotate(glm::mat4(1),glm::radians(rotationAngle),glm::vec3(1.0,0.0f,1.0f));
        mtxTransform = mtxProjection * mtxCamera * mtxTranslation * mtxRotation;

        rotationAngle += 1.0f;

        program.use();

        TextureManager::getInstance() -> activateTexture(GL_TEXTURE0, textureId);

        glBindVertexArray(VAO);

        program.setMat4ValueToUniform("uMtxTransform", &mtxTransform);
        
        // glDrawArray is using vertex buffer directly
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        std::this_thread::sleep_for (std::chrono::milliseconds(100));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
        
}
