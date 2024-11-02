#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D ourTexture;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 viewPosition;
uniform float ambient_k;
uniform float diffuse_k;
uniform float specular_k;
uniform float shininess;
uniform bool blinn;

void main()
{
    vec4 textureColor = texture(ourTexture, TexCoord);
    vec3 ambient = ambient_k * lightColor;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - FragPos); 
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff*diffuse_k) * lightColor;

    vec3 viewDir = normalize(viewPosition - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    vec3 specular;  

    if(blinn) {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
        specular = lightColor * spec;
    } else {
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        specular = specular_k * spec * lightColor; 
    }

    vec3 allLightForms = (ambient + diffuse + specular) * textureColor.rgb;
    FragColor = vec4(allLightForms, textureColor.a);
}