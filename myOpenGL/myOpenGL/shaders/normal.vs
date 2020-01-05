
#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 normMat;
uniform mat4 model;
uniform vec3 lightPos;
//uniform mat4 rotateMat;

out vec3 norm;
out vec2 TexCoords;
out vec3 fragPos;
out vec3 lightToPos;

void main(){
    gl_Position = projection * view * model * vec4(pos,1.0);
    norm = normalize(vec3(normMat * vec4(normal,1.0)));
    TexCoords = texCoords;
    fragPos = vec3(model * vec4(pos,1.0));
    lightToPos = fragPos - lightPos;
}


