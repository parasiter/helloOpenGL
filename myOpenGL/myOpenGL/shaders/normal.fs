#version 330 core

in vec2 TexCoords;
in vec3 fragPos;
in vec3 lightToPos;
in vec3 norm;
uniform sampler2D textureID;
uniform samplerCube textureCubeID;
uniform float far_plane;
uniform vec3 cameraPos;

out vec4 FragColor;

float getShadow(){
    float shadow = 0.0;
    float diskRadius = 0.05;
    vec3 sampleOffsetDirections[20] = vec3[]
    (
     vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1),
     vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
     vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
     vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
     vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
     );
    float fragDepth = length(lightToPos)/length(vec3(far_plane));
    for (int i = 0;  i < 20 ; ++i) {
        float texDepth = texture(textureCubeID,lightToPos + sampleOffsetDirections[i]*diskRadius).r;
        if (fragDepth - 0.0015 > texDepth) {
            shadow += 1.0;
        }
    }
    return shadow /= 20.0;
}

void main(){
    vec4 color = texture(textureID,TexCoords);
    //衰减系数
    float lightToPosDistance = length(lightToPos);
    float attenuation = 1.0/(1.0 + 0.007*lightToPosDistance + 0.0002*lightToPosDistance*lightToPosDistance);
    float ambientWeight = 0.2;
    
    float diffuseWeight = 0.8;
    float diffuseFactor = max(dot(normalize(lightToPos),-norm),0.);

    diffuseWeight *= diffuseFactor;
    
    float specWeight = 0.5;
    vec3 halfWayDir = normalize(normalize(cameraPos - fragPos) + normalize(-lightToPos));
    float specFactor = pow(dot(halfWayDir,norm),32.f);
    specWeight = specWeight * specFactor;
    
    vec3 finalColor = ambientWeight * vec3(color) + attenuation * (1.0- getShadow()) * (diffuseWeight *vec3(color) + specWeight * vec3(1.0));
    FragColor = vec4(finalColor,1.0);
}
