#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D ourTexture;
uniform sampler2D faceTexture;

void main()
{
    FragColor = mix(texture(ourTexture, TexCoord), texture(faceTexture, vec2(TexCoord.x, -TexCoord.y)), 0.2);
}