#include <iostream>

#include <glad/glad.h>
#include <GlFW/glfw3.h>

// vertex shader
char* vssource = 
"#version 330 core\n                          \
layout (location = 0) in vec3 inPosition;     \
void main()                                   \
{                                             \
    gl_Position = vec4(inPosition, 1.0f);     \
}";

// fragment shader
char* fssource =
"#version 330 core\n                          \
out vec4 fragColor;                           \
void main()                                   \
{                                             \
    fragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f); \
}";

float vertices[] = {
    -0.6f, -0.6f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

void errorCallback(int error, const char* description) {
    std::cerr << "GLFW error (" << error << "): " << description << std::endl;
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

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD!" << std::endl; 
        return -1;
    }

    // ------ CREATE PROGRAM ------
    unsigned int programId = glCreateProgram();

    unsigned int vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderId, 1, &vssource, NULL);
    glCompileShader(vertexShaderId);
    glAttachShader(programId, vertexShaderId);

    unsigned int fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderId, 1, &fssource, NULL);
    glCompileShader(fragmentShaderId);
    glAttachShader(programId, fragmentShaderId);

    glLinkProgram(programId);
    // ------ CREATE PROGRAM END ------

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

        glUseProgram(programId);
        glBindVertexArray(VAO);
        glEnableVertexAttribArray(0);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
        
}