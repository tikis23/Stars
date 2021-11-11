#pragma once

#include "Camera.h"
#include "Input.h"
#include "Window.h"

#include <glm/vec2.hpp>
#include "GlobalVariables.h"

class Player
{
public:
	Player(Window* window);
	~Player();
	void UpdateInput();
	void SmoothMove();
	glm::vec3 orbitPos;
	Camera* camera;
	float orbitDist = 50;
	float orbitSpeed = 2;
	float orbitMin = 1;
	float speed = 2;
	float sensitivity = 15;
	float scrollSensitivity = 2;
	glm::vec3 oldOrbitPos;
private:
	Window* window;
	glm::vec3 oldPos;
	glm::vec3 orbitPosLast;
	// input
	glm::vec2 previousMouse;
	float previousTime;
};