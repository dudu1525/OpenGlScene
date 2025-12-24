#include "Camera.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>//core glm functionality
#include <glm/gtc/matrix_transform.hpp>//glm extension for generating common transformation matrices
#include <glm/gtc/type_ptr.hpp>//glm extension for accessing the internal data structure of glm types

namespace gps {

    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp, float speed) {
        this->cameraPosition = cameraPosition;
        this->cameraTarget = cameraTarget;
        this->cameraUpDirection = cameraUp;
        this->cameraFrontDirection = glm::normalize(cameraTarget - cameraPosition); //vector from position to target
        this->cameraRightDirection =glm::normalize(glm::cross(cameraFrontDirection, cameraUpDirection));      
        this->speed = speed;
    }


    glm::mat4 Camera::getViewMatrix() {
        return  glm::lookAt(cameraPosition, cameraPosition + cameraFrontDirection, cameraUpDirection); 

    }

    void Camera::move(MOVE_DIRECTION direction, float deltaTime) {

        if (direction == MOVE_FORWARD)
        {
            cameraPosition += deltaTime* speed * cameraFrontDirection;
        }
        else if (direction == MOVE_BACKWARD)
        {
            cameraPosition -= deltaTime * speed * cameraFrontDirection;
        }
        else if (direction == MOVE_LEFT)
        {
            cameraPosition -= glm::normalize(glm::cross(cameraFrontDirection, cameraUpDirection)) * deltaTime * speed;
        }
        else
        {
            cameraPosition += glm::normalize(glm::cross(cameraFrontDirection, cameraUpDirection)) * deltaTime * speed;
        }



    }


    //yaw - camera rotation around the y axis
    //pitch - camera rotation around the x axis
    void Camera::rotate(float xoffset, float yoffset) {

            
        xoffset *= sensitivity;
        yoffset *= sensitivity;
        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;


        glm::vec3 direction;

        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        this->cameraFrontDirection = glm::normalize(direction);


        cameraRightDirection =
            glm::normalize(glm::cross(cameraFrontDirection, glm::vec3(0.0f, 1.0f, 0.0f)));

        cameraUpDirection =
            glm::normalize(glm::cross(cameraRightDirection, cameraFrontDirection));
    }

    void Camera::ProcessMouseScroll(float yoffset)
    {
        zoom -= yoffset;
        if (zoom < 1.0f)
            zoom = 1.0f;
        if (zoom > 45.0f)
            zoom = 45.0f;

    }



}