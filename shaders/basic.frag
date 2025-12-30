#version 410 core
out vec4 fColor;

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


in vec3 fPosWorld;
in vec3 fNormalWorld;
in vec2 fTexCoords;

uniform DirLight dirLight;
uniform vec3 viewPos;
// textures
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);



void main() 
{ 
//discard alpha channel
if(texture(diffuseTexture, fTexCoords).a < 0.1) {
        discard;
    }

    vec3 normal = normalize(fNormalWorld);
     vec3 viewDir = normalize(viewPos - fPosWorld); 

    vec3 lightResult = CalcDirLight(dirLight, normal, viewDir);

    fColor = vec4(lightResult, 1.0f);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
   
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

    vec3 texDiffuse = texture(diffuseTexture, fTexCoords).rgb;
    vec3 texSpecular = texture(specularTexture, fTexCoords).rgb;

    vec3 ambient  = light.ambient  * texDiffuse;
    vec3 diffuse  = light.diffuse  * diff * texDiffuse;
    vec3 specular = light.specular * spec * texSpecular;

    return (ambient + diffuse + specular);
}