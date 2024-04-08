#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <tool/shader.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <tool/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void transform(int TransType, Shader shader);

std::string Shader::dirName;

using namespace std;
int main(int argc, char *argv[])
{
  Shader::dirName = argv[1];
  glfwInit();
  // 设置主要和次要版本
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // 创建窗口对象
  GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  // 设置视口
  glViewport(0, 0, 800, 600);
  glEnable(GL_PROGRAM_POINT_SIZE);

  // 注册窗口变化监听
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  Shader ourShader("./Shader/vertex.vs", "./Shader/fragment.fs");

  // 顶点数据
  float vertices[] = {
      //---- 位置 ----  ---- 颜色 ----     - 纹理坐标 -
      0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // 右上
      0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 右下
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0, 0.0f, // 左下
      -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // 左上
  };

  // 索引数据
  unsigned int indices[] = {
      0, 1, 3, // 三角形一
      1, 2, 3  // 三角形二
  };

  // 创建缓冲对象
  unsigned int VBO, EBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // 绑定VAO缓冲对象
  glBindVertexArray(VAO);

  // 绑定VBO缓对象
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // 填充VBO数据
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // 设置顶点位置属性指针
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // 设置顶点颜色属性指针
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // 设置顶点纹理坐标属性指针
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);

  // 生成纹理
  unsigned int texture_box, texture_face;
  glGenTextures(1, &texture_box);
  glBindTexture(GL_TEXTURE_2D, texture_box);
  // 设置环绕和过滤方式
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // flip the y-axis of the texture
  stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

  int width, height, nrChannels;
  unsigned char *data = stbi_load("./static/texture/container.jpg", &width, &height, &nrChannels, 0);
  if (data)
  {
    // 这里是RGB，出错半个小时
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  stbi_image_free(data);

  glGenTextures(1, &texture_face);
  glBindTexture(GL_TEXTURE_2D, texture_face);

  // 设置环绕和过滤方式
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // 加载图片
  data = stbi_load("./static/texture/awesomeface.png", &width, &height, &nrChannels, 0);
  if (data)
  {
    // RGBA
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  stbi_image_free(data);

  ourShader.use();
  ourShader.setInt("texture_box", 0);
  ourShader.setInt("texture_face", 1);

  float NumMix = 0.0f;

  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    // 渲染指令
    // ...
    glClearColor(25.0 / 255.0, 85.0 / 255.0, 195.0 / 255.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    ourShader.use();

    NumMix = glfwGetTime();
    ourShader.setFloat("NumMix", NumMix);

    // 绑定纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_box);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_face);
    // 绘制物体
    glBindVertexArray(VAO); // 不需要每次都绑定，对于当前程序其实只需要绑定一次就可以了
    // glDrawArrays(GL_POINTS, 0, 6);
    // glDrawArrays(GL_LINE_LOOP, 0, 3);
    transform(0, ourShader);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    transform(1, ourShader);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }
}

void transform(int TransType, Shader shader)
{
  // 将变量声明从条件判断中提取出来
  unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform");
  glm::mat4 trans = glm::mat4(1.0f);

  switch (TransType)
  {
  case 0:
    // 初始化mat4
    trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
    trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
    trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
    break;

  case 1:
    // 初始化mat4
    float Time = abs(sin((float)glfwGetTime()));
    trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
    trans = glm::scale(trans, glm::vec3(Time, Time, 0.5f));
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
    break;
  default:
    break;
  }
}