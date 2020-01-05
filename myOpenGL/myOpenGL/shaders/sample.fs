
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 norm;
in vec2 TexCoord;

uniform vec3 observePos;

struct Material{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};
uniform  Material material;

struct SpotLight{
    vec3 lightPos;
    vec3 lightDir;
    float innerCutOff;
    float outerCutOff;
    float intensityFactor;
};

uniform SpotLight spotLight;

struct LightFactor{
    vec3 ambientFactor;
    vec3 diffFactor;
    vec3 specFactor;
};
uniform  LightFactor lightFactor;

void main()
{
    vec3 ambColorWeight = vec3(texture(material.texture_diffuse1,TexCoord)) * lightFactor.ambientFactor;

    float theta = dot(normalize(FragPos - spotLight.lightPos),normalize(spotLight.lightDir));
    float intensity =  clamp((theta - spotLight.outerCutOff)/(spotLight.innerCutOff - spotLight.outerCutOff),0.f,1.f);
    vec3 diffFactor = lightFactor.diffFactor * intensity*3.0f;
    vec3 specFactor = lightFactor.specFactor * intensity;
    //漫反射
    vec3 lightDir = normalize(spotLight.lightPos - FragPos);
    float diff = max(dot(lightDir,norm),0.0f);
    vec3 diffColorWeight = diff * vec3(texture(material.texture_diffuse1,TexCoord)) * diffFactor;
    //镜面光
    vec3 observeDir = normalize(observePos - FragPos);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(observeDir,reflectDir),0.0),material.shininess);
    vec3 specColorWeight = spec * vec3(texture(material.texture_specular1,TexCoord)) * specFactor;

    FragColor = vec4(ambColorWeight + diffColorWeight + specColorWeight,1.0f) ;
}
