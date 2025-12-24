#ifndef LightSources_h
#define LightSources_h
#if defined (__APPLE__)
#define GLFW_INCLUDE_GLCOREARB
#define GL_SILENCE_DEPRECATION
#else
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/matrix_inverse.hpp> 
#include <glm/gtc/type_ptr.hpp> 
using  glm::vec3;
//taken basically from learnopengl https://learnopengl.com/Lighting/Multiple-lights
namespace gps {

	class LightSources {
    public:

        LightSources();
        void setDirecLightParameters(vec3 direction, vec3 ambientD, vec3 diffuseD, vec3 specularD);
        void setLightUniforms(GLuint programId);

            
    private:
        struct DirLight {
            vec3 direction=vec3();
            vec3 ambient = vec3();
            vec3 diffuse = vec3();
            vec3 specular = vec3();
        };
        DirLight dirrlight;
        void setDirectionalUniforms(GLuint programId);



	};



}




#endif
