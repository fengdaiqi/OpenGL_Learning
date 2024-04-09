#version 330 core
out vec4 FragColor;
in vec2 outTexCoord;
uniform vec3 lightColor;

uniform sampler2D texture_box;

void main()
{
    FragColor = vec4(vec3(1.0f,0.9f,0.31f)*lightColor,1.0);
}
