#ifndef Fire_h
#define Fire_h
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
#include "Camera.hpp"


//inspired and done with: https://www.rastertek.com/gl4linuxtut33.html
//https://www.rastertek.com/gl4linuxtut34.html
class Fire {
public:
	void initializeFire(gps::Shader shader); //initialize the uniforms and inputs for the fire
	void renderFire(gps::Shader shader, glm::mat4 projection, gps::Camera camera, float deltaTime);


private:
	float frameTime=0.0f;
	glm::vec3 scrollSpeeds=glm::vec3(1.3f, 2.1f, 2.3f);
		glm::vec3 scales = glm::vec3(1.0,2.0f,3.0f);
	const char* fireTexture = "models/fire/fire01.tga";
	const char* noiseTexture = "models/fire/noise01.tga";
	const char* alphaTexture = "models/fire/alpha01.tga";
	glm::vec2 distortion1= glm::vec2(0.1f,0.2f), distortion2 = glm::vec2(0.1f,0.3f), distortion3= glm::vec2(0.1f,0.1f);
	float distortionScale=0.8f;
	float distortionBias = 0.5f;


	unsigned int fireVAO, fireVBO, fireEBO;
	unsigned int fireText;
	unsigned int alphaText, noiseText;
	float vertices[20] = {
	 4450.0f,    800.0f, 6300.0f,    0.0f, 1.0f,  //top left
	  4150.0f,    800.0f,  6300.0f,   1.0f,1.0f,// top right
		4150.0f,    300.0f,   6300.0f,   1.0f, 0.0f,// bottom right
		4450.0f,    300.0f,    6300.0f,   0.0f,0.0f,//bottom left
	};
	unsigned int indices[6] = {
		0,2,1, 
		0,3,2

	};

	float texCoords[8] = {
		0.0f, 1.0f,//top left
		1.0f,1.0f,//top right
		1.0f, 0.0f,//bottom right
		0.0f,0.0f//bottom left

	};


};


#endif;