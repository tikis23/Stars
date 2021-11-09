#pragma once

#include "Camera.h"
#include "Input.h"
#include "Window.h"

#include <glm/vec2.hpp>

class Player
{
public:
	Player(Window* window);
	~Player();
	void UpdateInput();
	glm::vec3 orbitPos;
	Camera* camera;
	float orbitDist = 50;
	float orbitSpeed = 2;
	float orbitMin = 1;
	float speed = 2;
	float sensitivity = 15;
private:
	Window* window;

	// input
	glm::vec2 previousMouse;
	float previousTime;
};