
#version 330 core
//layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 normal;
//layout (location = 2) in vec2 texCoord;
//
//uniform mat4 model;// 模型矩阵
//uniform mat4 transform; // 投影矩阵
//uniform mat4 normalRotate;
//
//out vec2 TexCoord;
//out vec3 norm;
//out vec3 FragPos;
//
//void main()
//{
//    gl_Position = transform * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
//    norm = vec3(normalRotate * vec4(normal,1.0f));
//    FragPos = vec3(model * vec4(aPos, 1.0));
//    TexCoord = texCoord;
//}

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
