#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec2 TexCoords;
out vec3 worldNormal;
out vec3 worldPos;
out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 shift;

void main()
{
    TexCoords = aTexCoords;
    
    vec3 T = normalize(vec3(model * vec4(aTangent,   0.0)));
    vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormal,    0.0)));
    TBN = mat3(T, B, N);

    worldPos = vec3(model * vec4(aPos, 1.0));
    worldNormal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projection * view * model *  vec4(aPos + shift, 1.0);
}