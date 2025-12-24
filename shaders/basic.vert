#version 410 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;
layout(location=2) in vec2 vTexCoords;

out vec3 fPosEye;
out vec3 fNormalEye;
out vec2 fTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

void main()
{
    gl_Position = projection * view * model * vec4(vPosition, 1.0);

    fPosEye = vec3(view * model * vec4(vPosition, 1.0));
    fNormalEye = normalize(normalMatrix * vNormal);

    fTexCoords = vTexCoords;
}