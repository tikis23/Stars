#pragma once
#include "noise/FastNoiseLite.h"
#include "Shader.h"

#include <GL/glew.h>

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GlobalVariables.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
};
class Planet
{
public:
	Planet(float i_size, float i_mult, glm::vec3 i_color, glm::vec3 i_planetRot, glm::vec3 i_atmoRot, int i_type, glm::vec3 i_ringColor = {}, bool i_hasRing = false);
	~Planet();
	void Draw(Shader* shader);
	void SetPos(glm::vec3 pos);
	glm::vec3 GetPos();
	float max_height = 1;
	float min_height = 0;
	float size;
	unsigned int rotIndex = 0;
private:
	struct _drawData_
	{
		unsigned int VBO, VAO;
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
			if (data.empty())
				return;
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, data.size());
		};
		void bind()
		{
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
		};
	};
	void subdivide(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, int detail, std::vector<Vertex>& data);
	void Generate();
private:
	FastNoiseLite noise;
	std::vector<_drawData_*> drawData;
	glm::vec3 pos;

	float atmoSize;
	glm::vec3 planetRot;
	glm::vec3 atmoRot;
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 modelAtmo = glm::mat4(1.0f);

	glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec4 colorAtmo = { 1.0f, 0.8f, 0.0f, 0.8f };
	glm::vec4 colorRing = { 1.0f, 0.8f, 0.0f, 0.8f };
	int detail = 6;

	bool isSun = false;
	bool isPlanet = false;
	bool isGas = false;
	bool isMoon = false;
	bool hasRing = false;

	float mult;
	bool generated = false;
};