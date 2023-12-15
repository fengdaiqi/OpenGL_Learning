#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window,int width,int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_HEIGHT=600;
const unsigned int SCR_WIDTH=800;

const char *vertexShaderSource ="#version 330 core\n"
        "layout (location=0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "       gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0);\n"
        "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 0.2f, 1.0f);\n"
    "}\n\0";


int main()
{
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
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
             

        //build and compile shader program
        //vertex shader
        unsigned int vertexShader=glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
        glCompileShader(vertexShader);

        //check for shader compile errors 
        int Success;
        char infolog[512];
        glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&Success);
        if(!Success)
        {
                glGetShaderInfoLog(vertexShader,512,NULL,infolog);
                std::cout<<"ERROR::SHADER::VERTEX::COMPLIATION_FAILED\n"<<infolog<<std::endl;
        }

        //fragment shader
        unsigned int fragmentShader=glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&Success);
        if(!Success)
        {
                glGetShaderInfoLog(fragmentShader,512,NULL,infolog);
                std::cout<<"ERROR::SHADER::FRAGMENT::COMPLIATION_FAILED\n"<<infolog<<std::endl;
        }

        //Link Shader into ShaderProgram
        unsigned int ShaderProgram;
        ShaderProgram=glCreateProgram();
        glAttachShader(ShaderProgram,vertexShader);
        glAttachShader(ShaderProgram,fragmentShader);
        glLinkProgram(ShaderProgram);
        glGetShaderiv(ShaderProgram,GL_LINK_STATUS,&Success);
        if(!Success)
        {
                glGetProgramInfoLog(ShaderProgram,512,NULL,infolog);
                std::cout<<"ERROR::SHADER::SHADERPROGRAM::LINK_FAILED\n"<<infolog<<std::endl;
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        //define vertices
        float vertices[] = {
                0.9f, 0.5f, 0.0f,   // 右上角
                0.9f, -0.5f, 0.0f,  // 右下角
                -0.5f, -0.5f, 0.0f, // 左下角
                -0.5f, 0.5f, 0.0f   // 左上角
        };
        unsigned int indices[]={
                //index starts from 0
                0,1,3,  //first triangle
                1,2,3   //second triangle
        };

        unsigned int VBO,VAO,EBO;
        glGenVertexArrays(1,&VAO);
        glGenBuffers(1,&VBO);
        glGenBuffers(1,&EBO);
        //bind VAO
        glBindVertexArray(VAO);
        //bind VBO to array_buffer
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

        //bind EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW); 

        //Link VertexAttrib
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);

        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

        //Render Loop
        while (!glfwWindowShouldClose(window))
        {
                processInput(window); 
                glClearColor(0.2f,0.3f,0.3f,1.0f);
                glClear(GL_COLOR_BUFFER_BIT); 

                glUseProgram(ShaderProgram);
                glBindVertexArray(VAO);
                //glDrawArrays(GL_TRIANGLES,0,3);
                glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
                
                //glBindVertexArray(0);

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

