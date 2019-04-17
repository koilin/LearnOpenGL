#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 matModel;
uniform mat4 matView;
uniform mat4 matProj;

void main()
{
    gl_Position = matProj * matView * matModel * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
