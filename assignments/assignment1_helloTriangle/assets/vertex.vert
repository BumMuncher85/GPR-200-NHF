#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
out vec4 Color;
uniform float time;
void main()
{
   Color = aColor;
   gl_Position = vec4(aPos.x, aPos.y + sin((time+aPos.x)*2.5)/5, aPos.z, 1.0);
};