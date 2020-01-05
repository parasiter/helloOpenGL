

#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in mat4 rockMat;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 normMat;
uniform mat4 modelMat;
uniform mat4 rotateMat;


out vec3 norm;
out vec2 TexCoords;

void main(){
    
    gl_Position = projection * view * modelMat * rotateMat * rockMat * vec4(pos,1.0);
    norm = vec3(normMat * vec4(normal,1.0));
    TexCoords = texCoords;
}
