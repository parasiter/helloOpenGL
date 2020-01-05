

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 normMat;
uniform mat4 model;
uniform mat4 lightMat;

out vec3 norm;
out vec2 TexCoords;
out vec3 fragPos;
out vec4 fragPosInLightSpace;

void main(){
    gl_Position = projection * view * model * vec4(aPos,1.0);
    fragPos = vec3(model * vec4(aPos,1.0));
    fragPosInLightSpace = lightMat * vec4(fragPos,1.0);
    norm = vec3(normMat * vec4(normal,1.0));
    TexCoords = texCoord;
}
