#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 vColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 meshColor;
uniform vec3 observerPosition;
uniform vec3 shift;

vec3 lightPos = vec3(5);
vec3 lightColor = vec3(1);


void main()
{
    //color = meshColor;
    vec3 V = normalize(aPos - observerPosition);
    vec3 N = aNormal;
    float ka = 0.3f;
    float kd = 0.015;
    float ks = 0.5f;
    int alpha = 10;
    vec3 diffColor = vec3(0);
    vec3 specColor = vec3(0);
    vec3 L = normalize(lightPos - aPos);
    
    float ln = dot(L,N);
    diffColor = ln*lightColor;
    
    vec3 R = normalize(2.0f*ln*N - L);
    
    float product = pow(dot(R,V), alpha);
    
    specColor = product*lightColor;
    vColor = ka*meshColor + kd*diffColor + ks*specColor;
    vColor.x = clamp(vColor.x, 0.0f, 1.0f);
    vColor.y = clamp(vColor.y, 0.0f, 1.0f);
    vColor.z = clamp(vColor.z, 0.0f, 1.0f);

    gl_Position = projection * view * model * vec4(aPos + shift, 1.0);
}