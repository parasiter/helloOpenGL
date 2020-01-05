
#version 330 core
layout (triangles) in;
layout (triangle_strip,max_vertices = 3) out;

in VS_OUT{
    vec2 texCoords;
    vec3 reflectDir;
} gs_in[];

out vec3 reflectDir;
out vec2 TexCoords;

uniform float time;

vec3 getNormal(){
    vec3 a = vec3(gl_in[0].gl_Position - gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position - gl_in[1].gl_Position);
    return normalize(cross(a,b));
}

vec3 getPointOffset(vec3 norm){
    float varyIndex = 1.0/6.0f;
    return norm * (sin(time) + 1) * varyIndex;
}

void main(){
    vec3 norm = getNormal();
    vec4 offSet = vec4(getPointOffset(norm),0.0);

    gl_Position = gl_in[0].gl_Position + offSet;
    TexCoords = gs_in[0].texCoords;
    reflectDir = gs_in[0].reflectDir;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position + offSet;
    TexCoords = gs_in[1].texCoords;
    reflectDir = gs_in[1].reflectDir;
    EmitVertex();

    gl_Position = gl_in[2].gl_Position + offSet;
    TexCoords = gs_in[2].texCoords;
    reflectDir = gs_in[2].reflectDir;
    EmitVertex();
    EndPrimitive();
}
