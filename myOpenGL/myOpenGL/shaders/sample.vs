
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 modelMat;// 模型矩阵
uniform mat4 cameraMat; // 投影矩阵
uniform mat4 normalMat; //法线变换矩阵

out vec2 TexCoord;
out vec3 norm;
out vec3 FragPos;

void main()
{
    gl_Position = cameraMat * modelMat * vec4(aPos, 1.0);
    norm = normalize(vec3(normalMat * vec4(normal,1.0f)));
    FragPos = vec3(modelMat * vec4(aPos, 1.0));
    TexCoord = texCoord;
}
