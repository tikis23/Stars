#pragma once
#include "noise/FastNoiseLite.h"
#include "Shader.h"

#include <GL/glew.h>

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GlobalVariables.h"
struct PlanetVariables
{
	int detail = 4;
	bool isSun = false;
	bool isPlanet = false;
	bool isGas = false;
	bool isMoon = false;
	bool hasRing = false;
	float max_height = 1;
	float min_height = 0;
	glm::vec3 maxColor = { 1, 1, 1 };
	glm::vec3 minColor = { 0, 0, 1 };
	float size = 1;
	float mult = 1000;
	float origin = 0;
	float dist = 0;
	float speed = 1;
	float ringSize = 3;
	glm::vec3 planetRot = { 1, 0, 0 };
	glm::vec3 ringRot = { 1, 0, 0 };
	glm::vec4 color = { 0.0f, 1.0f, 0.5f, 1.0f };
	glm::vec4 colorRing = { 1.0f, 0.5f, 0.8f, 0.95f };
};
struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
};
class Planet
{
public:
	Planet(float i_size, int i_detail, float i_mult, glm::vec3 i_color, glm::vec3 i_planetRot, glm::vec3 i_atmoRot, int i_type, glm::vec3 i_ringColor = {}, bool i_hasRing = false);
	Planet(PlanetVariables variables);
	Planet();
	~Planet();
	void Draw(Shader* shader);
	void DrawOrbit(Shader* shader);
	void SetPos(glm::vec3 pos);
	glm::vec3 GetPos();
	void Regenerate();
public:
	glm::mat4 modelOrbit = glm::mat4(1.0f);
	PlanetVariables variables;
private:
	struct _drawData_
	{
		unsigned int VBO, VAO, drawSize = 0;
		std::vector<Vertex>data;
		_drawData_()
		{
			glGenBuffers(1, &VBO);
			glGenVertexArrays(1, &VAO);
		};
		~_drawData_()
		{
			glDeleteBuffers(1, &VBO);
			glDeleteVertexArrays(1, &VAO);
		};
		void draw()
		{
			if (drawSize == 0)
				return;
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, drawSize);
		};
		void bind()
		{
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
		};
	};
	void subdivide(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, int detail, std::vector<Vertex>& data);
	void Generate();
	void GenOrbitData();
private:
	FastNoiseLite noise;
	std::vector<_drawData_*> drawData;
	glm::vec3 pos;

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 modelAtmo = glm::mat4(1.0f);

	bool generated = false;

	// orbit
	static _drawData_* orbitData;
};