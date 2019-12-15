
#version 330 core
out vec4 FragColor;

//in vec3 color;
in vec3 FragPos;
in vec3 norm;

uniform vec3 lightPos;
uniform vec3 observePos;

 uniform material{
    vec3 ambientColor;
    vec3 diffColor;
    vec3 specColor;
    float shininess;
 };
// uniform  Material material;

// struct LightFactor{
 uniform   vec3 ambientFactor;
 uniform   vec3 diffFactor;
 uniform   vec3 specFactor;
// };
// uniform  LightFactor lightFactor;

void main()
{
    vec3 ambColorWeight = ambientColor * ambientFactor;

    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(lightDir,norm),0.0f);
    vec3 diffColorWeight = diff * diffColor * diffFactor;
    
    vec3 observeDir = normalize(observePos - FragPos);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(observeDir,reflectDir),0.0),shininess);
    vec3 specColorWeight = spec * specColor * specFactor;
    
    FragColor = vec4(ambColorWeight + diffColorWeight + specColorWeight,1.0f) ;
}
