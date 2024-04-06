#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture_box;
uniform sampler2D texture_face;

uniform float NumMix;

void main()
{
    FragColor = mix(texture(texture_box,TexCoord),texture(texture_face,TexCoord),abs(sin(NumMix*5)));
 
    //FragColor= texture(texture1,TexCoord)*vec4(ourColor,1.0);
}