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
//effects constructed with the help of the videos: https://www.youtube.com/watch?v=HusvGeEDU_U&list=PLRIWtICgwaX23jiqVByUs0bqhnalNTNZh&index=4
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
    //CLIPPing plane must be at height -3.0, normal 0,1,0 or 0,-1,0, d = -3.0
	

};

class WaterFrameBuffers {
public:
    // Resolution constants (Adjust as needed)
    static const int REFLECTION_WIDTH = 1024;
    static const int REFLECTION_HEIGHT = 768;
    static const int REFRACTION_WIDTH = 1024;
    static const int REFRACTION_HEIGHT = 768;

    WaterFrameBuffers();
    ~WaterFrameBuffers();

    void bindReflectionFrameBuffer();
    void bindRefractionFrameBuffer();
    void unbindCurrentFrameBuffer();

    GLuint getReflectionTexture() { return reflectionTexture; }
    GLuint getRefractionTexture() { return refractionTexture; }
    GLuint getRefractionDepthTexture() { return refractionDepthTexture; }

private:
    GLuint reflectionFrameBuffer;
    GLuint reflectionTexture;
    GLuint reflectionDepthBuffer;

    GLuint refractionFrameBuffer;
    GLuint refractionTexture;
    GLuint refractionDepthTexture;

    void initialiseReflectionFrameBuffer();
    void initialiseRefractionFrameBuffer();

    void bindFrameBuffer(GLuint frameBuffer, int width, int height);
    GLuint createFrameBuffer();
    GLuint createTextureAttachment(int width, int height);
    GLuint createDepthTextureAttachment(int width, int height);
    GLuint createDepthBufferAttachment(int width, int height);
};

#endif





