
#version 330 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 offset;

out vec3 Color;

void main(){
    gl_Position = vec4(pos + offset,0.f,1.f);
    Color = color;
}
