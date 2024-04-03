#version 330 core

out vec4 FracColor;
in vec3 ourColor;
in vec3 ourPos;

void main()
{
  //FracColor = vec4(0.5f, 1.0f, 0.0f, 1.0f);
  //FracColor = vec4(ourColor, 1.0f);
  FracColor = vec4(ourPos, 1.0f);
}