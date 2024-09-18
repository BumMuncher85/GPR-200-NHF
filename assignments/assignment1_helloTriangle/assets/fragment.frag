#version 330 core
out vec4 FragColor;
in vec4 Color;
uniform float updateColor;
void main()
{
    FragColor = Color * updateColor;
} 