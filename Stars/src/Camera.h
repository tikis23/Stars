#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera(glm::vec3 position = glm::vec3{ 0, 0, -5 }, glm::vec3 front = glm::vec3{ 0, 0, 1 }, glm::vec3 up = glm::vec3{0, 1, 0}, float pNear = 0.0001f, float pFar = 10000000.0f, float fov = 90.0f);
	~Camera();
	void Update(float aspectRatio, bool orbit, glm::vec3 orbitPos = { 0, 0, 0 });
public:
	// position
	void SetPosition(glm::vec3 position);
	void OffsetPosition(glm::vec3 offset);
	glm::vec3& GetPosition();

	// rotation
	void SetRotation(float yaw, float pitch);
	void SetRotation(glm::vec3 front);
	void OffsetRotation(float yaw, float pitch);
	void OffsetRotation(glm::vec3 front);
	glm::vec3& GetRotation();
	void GetRotation(float& yaw, float& pitch);

	// planes
	void SetNearPlane(float near);
	void SetFarPlane(float far);
	float& GetNearPlane();
	float& GetFarPlane();

	// fov
	void SetFov(float fov);
	float& GetFov();

	// mat4's
	glm::mat4& GetViewMat();
	glm::mat4& GetProjMat();

	// else
	glm::vec3& GetFront();
	glm::vec3& GetUp();

private:
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec3 pos = { 0, 0, 0 };
	glm::vec3 front;
	glm::vec3 up;
	float yaw;
	float pitch;
	float aspectRatio;
	float nearPlane;
	float farPlane;
	float fov;
};