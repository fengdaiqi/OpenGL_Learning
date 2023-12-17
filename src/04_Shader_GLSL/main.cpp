#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <math.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void colorChange(int uniLocation);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "out vec4 vColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   vColor=vec4(0.1f,0.5f,0.0f,1.0f);\n"
    "}\0";
const char *fragmentShaderSource_1 = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec4 vColor;\n"
    "void main()\n"
    "{\n"
    "   vec2 uv=vec2(0.2f,0.8f);\n"
    "   vec4 color= vec4(uv.xy,0.0f,uv.y+uv.x);\n"
//    "   FragColor = vec4(color.x,color.y,color.z,color.w);\n"
    "   FragColor = vColor;\n"
    "}\n\0";

const char *fragmentShaderSource_2 = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 uniColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = uniColor;\n"
    "}\n\0";

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

        int nrAttributes;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
        std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader_1 = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int fragmentShader_2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader_1, 1, &fragmentShaderSource_1, NULL);
    glShaderSource(fragmentShader_2, 1, &fragmentShaderSource_2, NULL);
    glCompileShader(fragmentShader_1);
    glCompileShader(fragmentShader_2);
    // check for shader compile errors
    glGetShaderiv(fragmentShader_1, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader_1, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    glGetShaderiv(fragmentShader_2, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader_2, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    unsigned int shaderProgram_1 = glCreateProgram(),shaderProgram_2 = glCreateProgram();
    //First shader program
    glAttachShader(shaderProgram_1, vertexShader);
    glAttachShader(shaderProgram_1, fragmentShader_1);
    glLinkProgram(shaderProgram_1);
    //second shader program
    glAttachShader(shaderProgram_2, vertexShader);
    glAttachShader(shaderProgram_2, fragmentShader_2);
    glLinkProgram(shaderProgram_2);    

    // check for linking errors
    glGetProgramiv(shaderProgram_1, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram_1, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
        glGetProgramiv(shaderProgram_2, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram_2, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader_1);
    glDeleteShader(fragmentShader_2);
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices_First[] = {
        //first triangle
        -0.75f, -0.5f, 0.0f, // left  
        -0.25f, -0.5f, 0.0f, // right 
        -0.5f,  0.5f, 0.0f , // top   
    }; 

    float vertices_Second[] = {
        //second triangle
         0.25f, -0.5f, 0.0f, // left  
         0.75f, -0.5f, 0.0f, // right 
         0.5f,  0.5f, 0.0f  // top    
    }; 

    //set the VAO and VBO array
    unsigned int VBOS[2], VAOS[2];
    glGenVertexArrays(2, VAOS);
    glGenBuffers(2, VBOS);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAOS[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOS[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_First), vertices_First, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // bind the second VAO and VBO
    glBindVertexArray(VAOS[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOS[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_Second), vertices_Second, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    //get the uni variable location
    int uniColorLocation = glGetUniformLocation(shaderProgram_2,"uniColor");

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram_1);
        glBindVertexArray(VAOS[0]); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0); // no need to unbind it every time 
        glUseProgram(shaderProgram_2);
        glBindVertexArray(VAOS[1]); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);
        colorChange(uniColorLocation);  //change the uniform4f's value
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(2, VAOS);
    glDeleteBuffers(2, VBOS);
    glDeleteProgram(shaderProgram_1);
    glDeleteProgram(shaderProgram_2);  
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void colorChange(int uniLocation)
{
        // uniform color
    float timeValue = glfwGetTime();
    float greenValue = ((sin(timeValue*2)/2.0f)+0.5f);
    //get the uniform variable location
    glUniform4f(uniLocation,0.0f,greenValue,0.0f,1.0f);
}