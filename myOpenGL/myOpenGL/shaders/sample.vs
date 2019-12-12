
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texCoord;

out vec2 TexCoord;
uniform mat4 model;// 模型矩阵
uniform mat4 transform; // 投影矩阵

void main()
{
    gl_Position = transform * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    TexCoord = texCoord;
}
