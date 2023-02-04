#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 vColor;
uniform sampler2D texture_diffuse1;

void main()
{    
    FragColor = vec4(vec3(texture(texture_diffuse1, TexCoords)) + vColor,1.0f);
    //FragColor = vec4(vColor, 1.0f);
}