#ifndef Terrain_h
#define Terrain_h
#if defined (__APPLE__)
#define GLFW_INCLUDE_GLCOREARB
#define GL_SILENCE_DEPRECATION
#else
#define GLEW_STATIC
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/matrix_inverse.hpp> 
#include <glm/gtc/type_ptr.hpp> 


#include "Shader.hpp"
#include "LightSources.h"
#include "Camera.hpp"
#include <iostream>
#include <vector>

const unsigned int NUM_PATCH_PTS = 4;

class Terrain {

public:
#define WIDTH 48048
#define HEIGHT 48048
#define REZ 128


	Terrain() {}
	//new
	void initializeTerrain(const char* texturePath,const char* roughPath, gps::Shader terrainShader, gps::LightSources lights);
	void renderTerrain(gps::Shader terrainShader, glm::mat4 projection, gps::Camera camera);
	void setLightUniforms(gps::Shader terrainShader, gps::LightSources lights);

	glm::vec3 getNormal(float x, float z);
private:
	
	float perlinNoise(float x, float z);


	//new
	unsigned int terrainVAO, terrainVBO, terrainEBO;


	//int height=48048, width = 48048;//2048 - units of the map
	int maxh = 4449;//prev 7450, now 4450

	//const unsigned int rez = 128; //64x64 triangles that are further tessellated to max 1024 per vertex
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	//std::vector<float> heights;




	unsigned int sandTexture;
	unsigned int roughTexture;
	unsigned int normalmaptexture;
	const char* normals = "models/terrain/normals.png";

};




#endif