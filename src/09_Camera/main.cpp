#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <tool/shader.h>
#include <tool/camera.h>
#include <geometry/BoxGeometry.h>
#include <geometry/PlaneGeometry.h>
#include <geometry/SphereGeometry.h>

#define STB_IMAGE_IMPLEMENTATION
#include <tool/stb_image.h>

#include <tool/gui.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

void axisChange(Shader shader, BoxGeometry box);
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

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

  // Setup Dear ImGui context
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");
  float f = -3.0f;

  Shader ourShader("./Shader/vertex.vs", "./Shader/fragment.fs");

  // PlaneGeometry plane(1, 1, 1, 1);
  BoxGeometry box(1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
  glm::vec3 cubePositions[] = {
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),
      glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),
      glm::vec3(-1.3f, 1.0f, -1.5f)};
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

  glEnable(GL_DEPTH_TEST);

  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    // 渲染指令
    // ...
    glClearColor(25.0 / 255.0, 85.0 / 255.0, 195.0 / 255.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    {

      static int counter = 0;

      ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

      ImGui::SliderFloat("float", &f, 0.0f, 100.0f); // Create a slider that ranges from 0.0f to 1.0f
      ImGui::SameLine();
      ImGui::Text("counter = %d", counter);

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
      ImGui::End();
    }

    ourShader.use();

    // 绑定纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_box);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_face);
    // 绘制物体
    // 不需要每次都绑定，对于当前程序其实只需要绑定一次就可以了
    // glDrawArrays(GL_POINTS, 0, 6);
    // glDrawArrays(GL_LINE_LOOP, 0, 3);
    //  glm::mat4 model = glm::mat4(1.0f);
    //  model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-35.0f), glm::vec3(1.0f, 0.5f, 0.0f));
    //  // 别忘了绑定uniform
    //  shader.setMat4("model", model);
    glBindVertexArray(box.VAO);
    for (unsigned int i = 0; i < 10; i++)
    {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      float angle = 20.0f * i;
      int index = i % 3;
      switch (index)
      {
      case 0:
        model = glm::rotate(model, (float)glfwGetTime() + glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        break;

      default:
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        break;
      }

      ourShader.setMat4("model", model);
      glDrawElements(GL_TRIANGLES, box.indices.size(), GL_UNSIGNED_INT, 0);
    }

    float radius = 10.0f;
    float camX = sin(glfwGetTime()) * radius;
    float camZ = cos(glfwGetTime()) * radius;
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(cameraPos, cameraFront, cameraUp);
    ourShader.setMat4("view", view);

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);

    // glDrawElements(GL_POINTS, plane.indices.size(), GL_UNSIGNED_INT, 0);
    // glDrawElements(GL_LINE_LOOP, plane.indices.size(), GL_UNSIGNED_INT, 0);
    // glDrawElements(GL_TRIANGLES, box.indices.size(), GL_UNSIGNED_INT, 0);
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  // 释放资源
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  box.dispose();
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
  // 改变摄像机速度
  float cameraSpeed = 0.05f;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    cameraPos += cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    cameraPos -= cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
}
