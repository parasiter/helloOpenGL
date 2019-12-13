
#version 330 core
out vec4 FragColor;

//in vec3 color;
in vec3 FragPos;
in vec3 norm;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 observePos;

void main()
{
    vec4 modelColor = vec4(1.0,0.5,0.0,1.0f);
    float ambientFactor = 0.2f;
    vec3 lightDir = normalize(lightPos - FragPos);
    float diffFactor = max(dot(lightDir,norm),0.0f);
    
    vec3 observeDir = normalize(observePos - FragPos);
    vec3 reflectDir = reflect(-lightDir,norm);
    float specFactor = pow(max(dot(observeDir,reflectDir),0.0),32) * 0.5;
    
    
    FragColor = modelColor * vec4((ambientFactor + diffFactor)*lightColor, 1.0f) + vec4(specFactor*lightColor,1.0f) ;
}
