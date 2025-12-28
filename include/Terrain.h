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
	Terrain() {}
	//new
	void initializeTerrain(const char* texturePath, gps::Shader terrainShader, gps::LightSources lights);
	void renderTerrain(gps::Shader terrainShader, glm::mat4 projection, gps::Camera camera);

private:
	unsigned int heightmapTexture;
	unsigned char* heightmapData = nullptr;


	//new
	unsigned int terrainVAO, terrainVBO, terrainEBO;


	int height=2048, width = 2048;//2048 - units of the map
	int heightUP = 64;
	const unsigned int rez = 64; //64x64 triangles that are further tessellated to max 1024 per vertex
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	unsigned int sandTexture;


};




#endif