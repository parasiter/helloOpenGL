#version 330 core

in vec4 fragPos;

uniform vec3 lightPos;
uniform float far_plane;
out vec4 FragColor;
void main()
{
    float distance = length(fragPos.xyz - lightPos);
    float depth = distance/length(vec3(far_plane));
    gl_FragDepth = depth;
}
