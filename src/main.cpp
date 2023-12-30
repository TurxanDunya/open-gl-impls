#include <iostream>

#include <GlFW/glfw3.h>

int main()
{
    if (!glfwInit())
    {
       std::cout << "Error in initializing GLFW!" << std::endl; 
       return -1;
    }
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello OPENGL", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Error in creating GLFW window!" << std::endl; 
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
        
}