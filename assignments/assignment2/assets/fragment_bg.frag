#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;
uniform float fragTime;

void main()
{
    vec4 background = texture(ourTexture, vec2(TexCoord.x, TexCoord.y + sin(TexCoord.x + fragTime/2)/2));
    vec4 backgroundItem = texture(ourTexture2, TexCoord);
    FragColor = mix(background, backgroundItem, 0.0f);
}