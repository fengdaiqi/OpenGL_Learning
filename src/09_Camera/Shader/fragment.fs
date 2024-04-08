#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture_box;
uniform sampler2D texture_face;

void main()
{
    FragColor = mix(texture(texture_box,TexCoord),texture(texture_face,TexCoord),0.2);
    //FragColor= texture(texture_box,TexCoord);
}