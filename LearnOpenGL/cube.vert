#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
uniform mat4 matModel;
uniform mat4 matView;
uniform mat4 matProj;

out vec3 FragPos;
out vec2 TexCoords;

void main()
{
    gl_Position = matProj * matView * matModel * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    FragPos = vec3(matModel * vec4(aPos, 1.0));
    TexCoords = aTexCoord;
}
