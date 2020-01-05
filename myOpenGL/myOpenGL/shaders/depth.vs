
#version 330 core

layout (location = 0) in vec3 pos;
uniform mat4 model;
//out vec4 fragPos;

void main(){
    gl_Position = model * vec4(pos,1.0);
//    fragPos = model * vec4(pos,1.0);
}
