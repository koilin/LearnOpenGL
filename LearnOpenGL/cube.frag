#version 330 core
in vec3 FragPos;
in vec3 FragNormal;
in vec2 TexCoords;
out vec4 FragColor;

struct Material{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light{
    vec3 position;
    vec3 direction;
    float cutoff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    vec3 lightDir = normalize(light.position - FragPos);
    float theta = dot(lightDir, normalize(-light.direction));

    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

    vec3 norm = normalize(FragNormal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse * texture(material.diffuse, TexCoords).rgb;

    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 viewDir = normalize(viewPos - FragPos);
    float spec = max(dot(viewDir, reflectDir), 0.0);
    float specPow = pow(spec, material.shininess);
    vec3 specular = specPow * light.specular * texture(material.specular, TexCoords).rgb;
   
    vec3 result = ambient;
    if(theta > light.cutoff)
    {
        result = result + diffuse + specular;
    }
    
    FragColor = vec4(result, 1.0);
}