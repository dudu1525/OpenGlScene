#version 410 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;
layout(location=2) in vec2 vTexCoords;

out vec3 fPosWorld;    
out vec3 fNormalWorld;

out vec2 fTexCoords;

out vec4 fragPosLightSpace; //for shadows

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix; //transpose (inverse matrix)

uniform mat4 lightSpaceMatrix;//FOR SHADOWS

uniform vec4 plane;//for water shader set to 0, 0, 0 1 when not clipping!!!

void main()
{
    gl_Position = projection * view * model * vec4(vPosition, 1.0);
    fTexCoords = vTexCoords;
    //for water shader
    vec4 worldpos=model*vec4(vPosition, 1.0);
    gl_ClipDistance[0] = dot(worldpos,plane);
    
    
    //world space
    fPosWorld = vec3( model * vec4(vPosition, 1.0));

    fNormalWorld = normalize(normalMatrix * vNormal);

    fragPosLightSpace = lightSpaceMatrix * vec4(fPosWorld, 1.0);

}