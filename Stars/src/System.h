#pragma once
#include "Planet.h"
#include "Shader.h"
#include "GlobalVariables.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "Player.h"
class System
{
public:
	System(int mode = 0);
	~System();

	void Draw(Shader* shader);
	void DrawSuns(Shader* shader);
	void Update();
	Planet* Creator(Player* player);

	struct _orbit_
	{
		Planet* planet = nullptr;
		std::vector<Planet*>moons;
		void Update()
		{
			if (planet == nullptr)
				return;
			planet->modelOrbit = glm::translate(glm::mat4(1), { 0, 0, 0 });
			planet->modelOrbit = glm::scale(planet->modelOrbit, glm::vec3(planet->variables.dist, planet->variables.dist, planet->variables.dist));
			planet->SetPos({ planet->variables.dist * cos(glm::radians(planet->variables.origin + Global::time * planet->variables.speed)), 0,
				planet->variables.dist * sin(glm::radians(planet->variables.origin + Global::time * planet->variables.speed)) });
			for (auto& it : moons)
			{
				it->modelOrbit = glm::translate(glm::mat4(1), planet->GetPos());
				it->modelOrbit = glm::scale(it->modelOrbit, glm::vec3(it->variables.dist, it->variables.dist, it->variables.dist));
				it->SetPos(planet->GetPos() + it->variables.dist * glm::vec3{ cos(glm::radians(it->variables.origin + Global::time * it->variables.speed)), 0,
					sin(glm::radians(it->variables.origin + Global::time * it->variables.speed)) });
			}
		};
		void draw(Shader* shader)
		{
			if (planet != nullptr)
				planet->Draw(shader);
			for (auto& it : moons)
				it->Draw(shader);
		};
		_orbit_(Planet* planet = nullptr)
		{
			this->planet = planet;
		};
		~_orbit_()
		{
			if (planet != nullptr)
				delete planet;
			for (auto& it : moons)
				delete it;
		};
	};
public:
	std::vector<_orbit_*>orbits;
	std::vector<_orbit_*>suns;
};