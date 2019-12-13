#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;// 模型矩阵
uniform mat4 transform; // 投影矩阵

void main()
{
    gl_Position = transform * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}

