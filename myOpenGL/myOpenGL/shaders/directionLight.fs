#version 330 core

in vec2 TexCoords;
in vec3 fragPos;
in vec4 fragPosInLightSpace;

struct Material{
    sampler2D texture_diffuse1;
};
uniform  Material material;
uniform sampler2D depthBuffer;
out vec4 FragColor;

float getShadowFactor(){
    vec3 projCoords = fragPosInLightSpace.xyz/fragPosInLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(depthBuffer, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float shadow = currentDepth - 0.001 > closestDepth ? 0.0 : 1.0;
    if (projCoords.z > 1.0) {
        shadow = 1.0;
    }
    return shadow;
}

void main(){
    vec4 color = texture(material.texture_diffuse1,TexCoords);
    vec3 ambColorWeight = vec3(color) * 0.2;
    vec3 diffColorWeight = vec3(color) * 0.8;
    FragColor = vec4(getShadowFactor() * diffColorWeight + ambColorWeight,1.0);
}
