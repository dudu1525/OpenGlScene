#version 410 core
layout (location = 0) in vec3 aPos;


out vec4 clipSpace;
out vec2 textureCoords;//dudv map

out vec3 toCameraVector; //for fresnel

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 cameraPosition;//fresnel effect

void main()
{	
		vec4 worldpos=model*vec4(aPos, 1.0);

	gl_Position = projection * view * worldpos;

	clipSpace= projection * view * worldpos;
	textureCoords = vec2(aPos.x, aPos.z) / 4400.0;
	toCameraVector = cameraPosition - worldpos.xyz; //for fresnel
}