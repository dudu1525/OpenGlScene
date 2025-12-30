#ifndef Water_h
#define Water_h
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

class Water {
public:

	void initializeWater(gps::Shader shader);
	void renderWater(gps::Shader shader, glm::mat4 projection, gps::Camera camera);


private:
	unsigned int waterVAO, waterVBO, waterEBO;
	float vertices[18] = {
	 2200.0f,    -3.0f, -15000.0f, // Index 0 (Bottom)
	  460.0f,    -3.0f,  15000.0f, // Index 1 (Top)
	15000.0f,    -3.0f,   -400.0f, // Index 2 (Right)
   -15000.0f,    -3.0f,    900.0f
	};//was 660, when height was 7450

	

};
/// <summary>  rectangle for palm trees generation, x z
//palm trees bounded by: z: -6500, 6500, x: -5500, 5500 and z: +-13k, x: +-12k
/// </summary>
class WaterFrameBuffer {





};






#endif