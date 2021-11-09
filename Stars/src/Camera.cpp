#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float pNear, float pFar, float fov)
{
    this->pos = position;
    this->front = front;
    this->up = up;
    this->nearPlane = pNear;
    this->farPlane = pFar;
    this->fov = fov;
    this->yaw = 90;
    this->pitch = 0;
    SetRotation(this->yaw, this->pitch);
}

Camera::~Camera()
{

}

void Camera::Update(float aspectRatio, bool orbit, glm::vec3 orbitPos)
{
    projection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    if(orbit)
        view = glm::lookAt(pos, orbitPos, up);
    else
        view = glm::lookAt(pos, pos + front, up);
}

// Position
void Camera::SetPosition(glm::vec3 position)
{
    pos = position;
}

void Camera::OffsetPosition(glm::vec3 offset)
{
    pos += offset;
}

glm::vec3& Camera::GetPosition()
{
    return pos;
}

// Rotation
void Camera::SetRotation(float yaw, float pitch)
{
    this->yaw = yaw;
    this->pitch = pitch;
    if (this->yaw > 360.0f)
        this->yaw = 0;
    if (this->yaw < 0.0f)
        this->yaw = 360;
    if (this->pitch > 89.0f)
        this->pitch = 89.0f;
    if (this->pitch < -89.0f)
        this->pitch = -89.0f;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
}

void Camera::SetRotation(glm::vec3 front)
{
    float p = glm::degrees(asin(front.y));
    float y = glm::degrees(acos(front.x / cos(glm::radians(p))));
    SetRotation(y, p);
}

void Camera::OffsetRotation(float yaw, float pitch)
{
    this->yaw += yaw;
    this->pitch += pitch;
    if (this->yaw > 360.0f)
        this->yaw = 0;
    if (this->yaw < 0.0f)
        this->yaw = 360;
    if (this->pitch > 89.0f)
        this->pitch = 89.0f;
    if (this->pitch < -89.0f)
        this->pitch = -89.0f;
    front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    front.y = sin(glm::radians(this->pitch));
    front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    front = glm::normalize(front);
}

void Camera::OffsetRotation(glm::vec3 front)
{
    float p = glm::degrees(asin(front.y));
    float y = glm::degrees(acos(front.x / cos(glm::radians(p))));
    OffsetRotation(y, p);
}

glm::vec3& Camera::GetRotation()
{
    return front;
}

void Camera::GetRotation(float& yaw, float& pitch)
{
    yaw = this->yaw;
    pitch = this->pitch;
}

// Planes
void Camera::SetNearPlane(float near)
{
    nearPlane = near;
}

void Camera::SetFarPlane(float far)
{
    farPlane = far;
}

float& Camera::GetNearPlane()
{
    return nearPlane;
}

float& Camera::GetFarPlane()
{
    return farPlane;
}

// FOV
void Camera::SetFov(float fov)
{
    this->fov = fov;
}

float& Camera::GetFov()
{
    return fov;
}

// Mat4's
glm::mat4& Camera::GetViewMat() 
{
    return view;
}

glm::mat4& Camera::GetProjMat()
{
    return projection;
}

glm::vec3& Camera::GetFront()
{
    return front;
}

glm::vec3& Camera::GetUp()
{
    return up;
}

