#ifndef Camera_hpp
#define Camera_hpp

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace gps {
    
    enum MOVE_DIRECTION {MOVE_FORWARD, MOVE_BACKWARD, MOVE_RIGHT, MOVE_LEFT};
    //from learnOpenGL.com
    class Camera {

    public:
        float zoom = 45.0f;
        Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp, float speed);
        //return the view matrix, using the glm::lookAt() function
        glm::mat4 getViewMatrix();
        //update the camera internal parameters following a camera move event
        void move(MOVE_DIRECTION direction, float deltaTime);
        //update the camera internal parameters following a camera rotate event
        //yaw - camera rotation around the y axis
        //pitch - camera rotation around the x axis
        void rotate(float xoffset, float yoffset);

        void ProcessMouseScroll(float yoffset);

        void invertPitch() {
            this->pitch = -pitch;
            updateCameraVectors();
        }

        void setPosition(glm::vec3 newPos) {
            this->cameraPosition = newPos;
        }

        void updateCameraVectors() {
            glm::vec3 front;
            front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            front.y = sin(glm::radians(pitch));
            front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            this->cameraFrontDirection = glm::normalize(front);

            this->cameraRightDirection = glm::normalize(glm::cross(this->cameraFrontDirection, glm::vec3(0,1,0)));
            this->cameraUpDirection = glm::normalize(glm::cross(this->cameraRightDirection, this->cameraFrontDirection));

        }


        glm::vec3 getPositionCamera() { return this->cameraPosition; }
    private:
        glm::vec3 cameraPosition;
        glm::vec3 cameraTarget;
        glm::vec3 cameraFrontDirection;
        glm::vec3 cameraRightDirection;
        glm::vec3 cameraUpDirection;
        float yaw = -90.0f;
        float pitch = 0.0f;
        
        float speed = 0.15f;

        const float sensitivity = 0.1f;

    };    
}

#endif /* Camera_hpp */
