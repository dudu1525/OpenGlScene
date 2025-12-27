#ifndef Entity_h
#define Entity_h
#if defined (__APPLE__)
#define GLFW_INCLUDE_GLCOREARB
#define GL_SILENCE_DEPRECATION
#else
#define GLEW_STATIC
#include <GL/glew.h>
#endif
#include <iostream>
#include <string.h>
#include <vector>

#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Model3D.hpp"

class Entity {
public:
    gps::Model3D* model=nullptr;
    glm::vec3 position;
    glm::vec3 rotation;
    float scale;
    Entity() {}
    Entity(gps::Model3D* m, glm::vec3 pos) : model(m), position(pos), rotation(0, 0, 0), scale(1.0f) {}

    glm::mat4 getModelMatrix() {
        glm::mat4 m = glm::translate(glm::mat4(1.0f), position);
        m = glm::rotate(m, glm::radians(rotation.x), glm::vec3(1, 0, 0));
        m = glm::rotate(m, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        m = glm::rotate(m, glm::radians(rotation.z), glm::vec3(0, 0, 1));
        m = glm::scale(m, glm::vec3(scale));
        return m;
    }

    gps::Model3D* getModel() {
        return model;
    }
};


#endif