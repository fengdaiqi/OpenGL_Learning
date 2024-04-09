#version 330 core
out vec4 FragColor;
in vec2 outTexCoord;
in vec3 outNormal;
in vec3 outFragPos;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform sampler2D texture_box;

void main()
{
    vec4 objectColor = texture(texture_box, outTexCoord);

    float ambientStrength = 0.1;
    vec3 ambient= ambientStrength*lightColor;

    vec3 norm = normalize(outNormal);
    vec3 lightDir = normalize(lightPos-outFragPos);

    float diff = max(dot(norm,lightDir),0.0f);
    vec3 diffuse = diff*lightColor;
    
    vec3 result= (diffuse+ambient)*vec3(objectColor);   
    FragColor = vec4(result,1.0);
   
}
