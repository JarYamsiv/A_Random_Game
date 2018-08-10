#ifndef _CAMERA_H
#define _CAMERA_H

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


enum CAM_DIRN
{
    CAM_FORWARD,
    CAM_BACKWARD,
    CAM_LEFT,
    CAM_RIGHT
};

class Camera{

public:
                                Camera(glm::vec3, glm::vec3, float, float);
                                ~Camera();
    void                        ProcessKeyboard(CAM_DIRN direction,float deltaTime);
    void                        ProcessMouseMovement(float,float,GLboolean);
    glm::mat4                   getViewMat();
private:
    void                        UpdateCameraVectors();
    // Camera Attributes
    glm::vec3                   Position;
    glm::vec3                   Front;
    glm::vec3                   Up;
    glm::vec3                   Right;
    glm::vec3                   WorldUp;
    // Euler Angles
    float                       Yaw;
    float                       Pitch;
    // Camera options
    float                       MovementSpeed;
    float                       MouseSensitivity;
    float                       Zoom;

};



#endif