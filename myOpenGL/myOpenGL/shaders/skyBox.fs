
#version 330 core
out vec4 FragColor;
in vec3 TexCoords;

uniform samplerCube textureID;

void main()
{
    float depth = texture(textureID, TexCoords).r;
    FragColor = vec4(depth);
    
//    FragColor = vec4(1.0);
}
