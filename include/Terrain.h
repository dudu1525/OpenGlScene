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
#include <iostream>
#include <vector>

const unsigned int NUM_PATCH_PTS = 4;

class Terrain {

public:
	Terrain() {}
	//new
	void initializeTerrain(const char* heightmapLocation, gps::Shader terrainShader);
	void renderTerrain(gps::Shader terrainShader, glm::mat4 projection, glm::mat4 view);

private:



	//new
	unsigned int terrainVAO, terrainVBO;
	unsigned int heightmapTexture;
	unsigned char* heightmapData = nullptr;
	int height=0, width = 0, nrChannels;
	const unsigned int rez = 50;
	std::vector<float> vertices;


};




#endif