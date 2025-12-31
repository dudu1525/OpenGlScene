#version 410 core
out vec4 fColor;

struct DirLight {
    vec3 direction;
	
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
uniform Material material;
uniform vec3 viewPos;

// textures of object
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

    fColor = vec4(lightResult, material.opacity);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
   
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 texDiffuse = texture(diffuseTexture, fTexCoords).rgb;
    vec3 texSpecular = texture(specularTexture, fTexCoords).rgb;

    vec3 ambient  = light.ambient  * texDiffuse *  material.ambient;
    vec3 diffuse  = light.diffuse  * diff * texDiffuse* material.diffuse;
    vec3 specular = light.specular * spec * texSpecular * material.specular;

    return (ambient + diffuse + specular);
}