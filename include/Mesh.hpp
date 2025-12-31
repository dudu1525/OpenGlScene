#ifndef Mesh_hpp
#define Mesh_hpp

#if defined (__APPLE__)
    #define GL_SILENCE_DEPRECATION
    #include <OpenGL/gl3.h>
#else
    #define GLEW_STATIC
    #include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include "Shader.hpp"

#include <string>
#include <vector>


namespace gps {

    struct Vertex {

        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };

    struct Texture {

        GLuint id;
        //ambientTexture, diffuseTexture, specularTexture, roughnessTexture
        std::string type;
        std::string path;
    };

    struct Material {

        glm::vec3 ambient;//Ka
        glm::vec3 diffuse;//Kd
        glm::vec3 specular;//Ks
        float shininess;//Ns
        float refraction;//Ni
        float opacity;//d

    };

    struct Buffers {
        GLuint VAO;
        GLuint VBO;
        GLuint EBO;
    };

    class Mesh {

    public:
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<Texture> textures;
        Material materials;

	    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures, Material materials);

	    Buffers getBuffers();

	    void Draw(gps::Shader shader);

    private:
        /*  Render data  */
        Buffers buffers;

	    // Initializes all the buffer objects/arrays
	    void setupMesh();

    };

}
#endif /* Mesh_hpp */
