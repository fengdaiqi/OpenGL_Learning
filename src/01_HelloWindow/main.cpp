#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window,int width,int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_HEIGHT=600;
const unsigned int SCR_WIDTH=800;

int main()
{
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GLFW_TRUE);

        //create a window
        GLFWwindow* window = glfwCreateWindow(SCR_WIDTH,SCR_HEIGHT,"LearnOpenGL",NULL,NULL);
        if (window == NULL)
        {
                std::cout<<"Failed to create a window"<<std::endl;
                glfwTerminate();
                return -1;
        }
        glfwMakeContextCurrent(window); 
        //initial glad
        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
                std::cout<<"Failed to initial GLAD"<<std::endl;
                return -1;
        }
        glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);       
        //Render Loop
        while (!glfwWindowShouldClose(window))
        {
                processInput(window); 
                glClearColor(0.5f,0.0f,0.0f,1.0f);
                glClear(GL_COLOR_BUFFER_BIT); 
                glfwSwapBuffers(window);
                glfwPollEvents();
        }       
        glfwTerminate();
        return 0;

}

//CallBack Function
void framebuffer_size_callback(GLFWwindow* window,int width,int height)
{
        glViewport(0,0,width,height);
}

void processInput(GLFWwindow* window)
{
        if(glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS)
                glfwSetWindowShouldClose(window,true);
}

