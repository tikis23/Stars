#pragma once

#include "Shader.h"
#include "Player.h"
#include "Window.h"
#include "Skybox.h"
#include "System.h"

#include <GL/glew.h>
class Renderer
{
public:
	Renderer(Window* window);
	~Renderer();
	void Draw(Shader* fboDrawShader, unsigned int fbo, unsigned int fbovao, unsigned int fboTexture);

private:
	void Blur(unsigned int texture);

	System* system;
	Shader* shader;
	Shader* compute;
	Shader* threshold;

	Planet* orbitPlanet;
	Player* player;
	Window* window;
	Skybox* skybox;
	unsigned int output;

};