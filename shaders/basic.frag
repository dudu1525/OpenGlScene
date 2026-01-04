#version 410 core
out vec4 fColor;

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

//properties given by materials of object
struct Material {
     vec3 ambient;//Ka
     vec3 diffuse;//Kd
     vec3 specular;//Ks
     float shininess;//Ns
     float refraction;//Ni
     float opacity;//d
    };


in vec3 fPosWorld;
in vec3 fNormalWorld;
in vec2 fTexCoords;

uniform DirLight dirLight;
uniform PointLight pointlight;
uniform Material material;
uniform vec3 viewPos;

// textures of object
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
//uniform sampler2D roughnessTexture;

uniform sampler2D shadowMap;//for shadows
in vec4 fragPosLightSpace;//for shadows

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal,vec3 fragPos, vec3 viewDir);

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fNormalWorld);
    vec3 lightDir = normalize(-dirLight.direction);
float bias = max(0.0005 * (1.0 - dot(normal, lightDir)), 0.00005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

void main() 
{ 
//discard alpha channel
if(texture(diffuseTexture, fTexCoords).a < 0.1) {
        discard;
    }

    vec3 normal = normalize(fNormalWorld);
     vec3 viewDir = normalize(viewPos - fPosWorld); 

     float shadow = ShadowCalculation(fragPosLightSpace);
     vec3 dirLightColor = CalcDirLight(dirLight, normal, viewDir);
   // vec3 lightResult = CalcDirLight(dirLight, normal, viewDir);

    vec3 texDiffuse = texture(diffuseTexture, fTexCoords).rgb;
    vec3 ambientPart = dirLight.ambient * texDiffuse * material.ambient;
    //initially 1.0 = shadow, 0=no shadow
    vec3 dirLightFinal = ambientPart + (1.0 - shadow) * (dirLightColor - ambientPart);
    //^combine the ambient value from the texture with the 'deleted' diffuse texture

  //  lightResult += CalcPointLight(pointlight,normal,fPosWorld,viewDir  );
vec3 finalResult = dirLightFinal + CalcPointLight(pointlight, normal, fPosWorld, viewDir);
   // fColor = vec4(lightResult, material.opacity);
    fColor = vec4(finalResult, material.opacity);

}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
   
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    
    //float roughness = texture(roughnessTexture, fTexCoords).r;


    vec3 texDiffuse = texture(diffuseTexture, fTexCoords).rgb;
    vec3 texSpecular = texture(specularTexture, fTexCoords).rgb;

    vec3 ambient  = light.ambient  * texDiffuse *  material.ambient;
    vec3 diffuse  = light.diffuse  * diff * texDiffuse* material.diffuse;
    vec3 specular = light.specular * spec * texSpecular * material.specular;

    return (ambient + diffuse + specular);
}
//from learn opengl. com
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    

    vec3 texDiffuse = texture(diffuseTexture, fTexCoords).rgb;
    vec3 texSpecular = texture(specularTexture, fTexCoords).rgb;

    vec3 ambient  = light.ambient  * texDiffuse * material.ambient;
    vec3 diffuse  = light.diffuse  * diff * texDiffuse * material.diffuse;
    vec3 specular = light.specular * spec * texSpecular * material.specular;
    return (ambient + diffuse + specular)*attenuation;
} 