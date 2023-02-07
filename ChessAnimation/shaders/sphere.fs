#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;


in vec3 vColor;
void main()
{
    FragColor = vec4(vColor,1.0f);
}