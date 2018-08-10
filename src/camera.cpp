#include "../headers/camera.h"

Camera::Camera(
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
    float yaw=-90.0,
    float pitch=0.0)
{
    Front = glm::vec3(0.0f, 0.0f, -1.0f);
    Position = position;
    Up = up;
    Yaw = yaw;
    Pitch = pitch;
    UpdateCameraVectors();
    MovementSpeed=2.5;
    MouseSensitivity=0.1;
}

Camera::~Camera()
{
}

void Camera::ProcessKeyboard(CAM_DIRN direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == CAM_FORWARD)
        Position += Front * velocity;
    if (direction == CAM_BACKWARD)
        Position -= Front * velocity;
    if (direction == CAM_LEFT)
        Position -= glm::normalize(glm::cross(Front, Up)) * velocity;
    if (direction == CAM_RIGHT)
        Position += glm::normalize(glm::cross(Front, Up)) * velocity;

    //UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
    // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // Also re-calculate the Right and Up vector
        //Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        //Up    = glm::normalize(glm::cross(Right, Front));
}

glm::mat4 Camera::getViewMat()
{
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // Update Front, Right and Up Vectors using the updated Euler angles
        UpdateCameraVectors();
    }