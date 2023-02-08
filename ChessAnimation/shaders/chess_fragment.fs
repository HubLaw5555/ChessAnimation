#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 worldPos;
in vec3 worldNormal;
in mat3 TBN;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;  

vec3 lightColor = vec3(1);
vec3 k = vec3(0.45, 0.6, 0.15);
int n = 20;

uniform vec3 l1, l2, l3, l4;
uniform vec3 cameraPos;
uniform float fogMultiplier;

float fogIntensity = 1;

float CalcFogFactor(vec3 pos)
{
    float gradient = (fogIntensity * fogIntensity - 50 * fogIntensity + 60);
    float distance = length(cameraPos - pos);
    float fog = exp(-pow((distance*fogMultiplier/ gradient), 4));
    fog = clamp(fog, 0.0, 1.0);
    return fog;
}

vec3 CalcLight(vec3 light, vec3 normal, vec3 pos)
{
    vec3 L = normalize(light - pos);
    vec3 V = normalize(cameraPos - pos);
    vec3 N = normal;
    vec3 diffuse = dot(L,N)*lightColor;
    vec3 R = normalize(2.0f * dot(L, N) * N - L);
    float product = max(0.0f, dot(R, V));
    if (product > 0.0f)
    {
        product = pow(product, n);
    }
    vec3 specular = product*lightColor;
    return k.y*diffuse + k.z*specular;
}

void main()
{    
    vec3 finalColor = vec3(0.0);
    vec3 lights[4] = vec3[](l1, l2, l3, l4);
    vec3 normal = texture(texture_normal1, TexCoords).rgb;
    normal = normal * 2.0 - 1.0;   
    normal = normalize(TBN * normal);

    for(int i = 0; i < 1; i++)
    {
        finalColor = finalColor + CalcLight(lights[i], normal, worldPos);
    }
    finalColor = clamp(finalColor, 0.0, 1.0);
    float fog_factor = CalcFogFactor(worldPos);
    vec3 fogColor = vec3(1.0);
    finalColor = mix(fogColor, finalColor, fog_factor);
    
    FragColor = vec4(k.x*vec3(texture(texture_diffuse1, TexCoords)) + finalColor,1.0f);
    //FragColor = vec4(vColor, 1.0f);
}