#version 330 core
out vec4 FragColor;
in vec2 outTexCoord;
in vec3 outNormal;
in vec3 outFragPos;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

struct Light{
    vec3 postion;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

};
uniform Light light;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform sampler2D texture_box;
uniform vec3 viewPos;



void main()
{
    vec4 objectColor = texture(texture_box, outTexCoord);

    //环境光照
    vec3 ambient= material.ambient*light.ambient;
    //漫反射光照
    vec3 norm = normalize(outNormal);
    vec3 lightDir = normalize(light.postion-outFragPos);
    float diff = max(dot(norm,lightDir),0.0f);
    vec3 diffuse = (diff*material.diffuse)*light.diffuse;
    //镜面光照
    vec3 viewDir = (normalize(viewPos-outFragPos));
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),32);
    vec3 specular = (material.specular*spec)*light.specular;

    vec3 result= diffuse+ambient+specular;   
    FragColor = vec4(result,1.0);
   
}
