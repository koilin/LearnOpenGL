#version 330 core
in vec3 FragPos;
in vec3 FragNormal;
out vec4 FragColor;

struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform Material material;

void main()
{
    vec3 ambient = lightColor * material.ambient;

    vec3 norm = normalize(FragNormal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * material.diffuse;

    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 viewDir = normalize(viewPos - FragPos);
    float spec = max(dot(viewDir, reflectDir), 0.0);
    float specPow = pow(spec, 256);
    vec3 specular = specPow * lightColor * material.specular;
   
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}