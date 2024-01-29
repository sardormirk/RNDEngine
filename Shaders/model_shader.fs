#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


#define NR_POINT_LIGHTS 1

out vec4 FragColor;

in vec2 TexCoords;
in vec3 normal;
in vec3 fragPos;

uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;
uniform vec3 viewPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

uniform bool lightsOn;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec4 texDiff, vec4 texSpec);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 texDiff, vec4 texSpec);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{    
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec4 texDiffuse = texture(texture_diffuse1, TexCoords);
    vec4 texSpecular = texture(texture_specular1, TexCoords);
    vec4 texNormal = texture(texture_normal1, TexCoords);
    vec4 texHeight = texture(texture_height1, TexCoords);

    vec3 resultDir = CalcDirLight(dirLight, norm, viewDir, texDiffuse, texSpecular);

    vec3 resultPoint = CalcPointLight(pointLights[0], norm, fragPos, viewDir, texDiffuse, texSpecular);

    vec4 tex = texDiffuse * texSpecular * texNormal * texHeight;
    vec4 fResult = vec4(resultDir + resultPoint, 1.0f);

    if(!lightsOn){
        fResult = vec4(resultDir, 1.0f);
    }

    FragColor = mix(tex, fResult, 0.5f);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec4 texDiff, vec4 texSpec)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    // combine results
    vec3 ambient = light.ambient * texDiff.rgb;
    vec3 diffuse = light.diffuse * diff * texDiff.rgb;
    vec3 specular = light.specular * spec * texSpec.rgb;
    return (ambient + diffuse + specular);
}
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 texDiff, vec4 texSpec)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient * texDiff.rgb;
    vec3 diffuse = light.diffuse * diff * texDiff.rgb;
    vec3 specular = light.specular * spec * texSpec.rgb;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * material.diffuse;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    ambient *= attenuation*intensity;
    diffuse *= attenuation *intensity;
    specular *= attenuation *intensity;
    return (ambient + diffuse + specular);
}
