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
	System(bool sun = false);
	~System();

	void Draw(Shader* shader);
	void DrawSuns(Shader* shader);
	void Update();
	Planet* Creator(Player* player);
private:
	struct _orbit_
	{
		Planet* planet = nullptr;
		std::vector<Planet*>moons;
		void Update()
		{
			if (planet == nullptr)
				return;
			planet->modelOrbit = glm::translate(glm::mat4(1), { 0, 0, 0 });
			planet->modelOrbit = glm::scale(planet->modelOrbit, glm::vec3(planet->dist, planet->dist, planet->dist));
			planet->SetPos({ planet->dist * cos(glm::radians(planet->origin + Global::time * planet->speed)), 0,
				planet->dist * sin(glm::radians(planet->origin + Global::time * planet->speed)) });
			for (auto& it : moons)
			{
				it->modelOrbit = glm::translate(glm::mat4(1), planet->GetPos());
				it->modelOrbit = glm::scale(it->modelOrbit, glm::vec3(it->dist, it->dist, it->dist));
				it->SetPos(planet->GetPos() + it->dist * glm::vec3{ cos(glm::radians(it->origin + Global::time * it->speed)), 0,
					sin(glm::radians(it->origin + Global::time * it->speed)) });
			}
		};
		void draw(Shader* shader)
		{
			if (planet != nullptr)
				planet->Draw(shader);
			for (auto& it : moons)
				it->Draw(shader);
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
	std::vector<Planet*>suns;
};