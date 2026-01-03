#version 410 core
layout (location = 0) in vec3 inputPosition;
layout (location = 1) in vec2 inputTexCoord;
//layout (location = 2) in vec3 inputNormal; not needed?

out vec2 texCoord; //initial texture coords

out vec2 texCoords1;
out vec2 texCoords2;
out vec2 texCoords3;

uniform float frameTime; //used to scroll up the textures
uniform vec3 scrollSpeeds; //3 different scrolling speeds for 3 textures
uniform vec3 scales; //for outputting 3 types of the same texture

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(inputPosition, 1.0);

	 texCoord = inputTexCoord;

    // Compute texture coordinates for first noise texture using the first scale and upward scrolling speed values.
    texCoords1 = (inputTexCoord * scales.x);
    texCoords1.y = texCoords1.y - (frameTime * scrollSpeeds.x);

    // Compute texture coordinates for second noise texture using the second scale and upward scrolling speed values.
    texCoords2 = (inputTexCoord * scales.y);
    texCoords2.y = texCoords2.y - (frameTime * scrollSpeeds.y);

    // Compute texture coordinates for third noise texture using the third scale and upward scrolling speed values.
    texCoords3 = (inputTexCoord * scales.z);
    texCoords3.y = texCoords3.y - (frameTime * scrollSpeeds.z);


}