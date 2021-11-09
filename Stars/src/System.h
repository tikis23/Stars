#pragma once
#include "Planet.h"
#include "Shader.h"
#include "GlobalVariables.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
class System
{
public:
	System(bool sun = false);
	~System();

	void Draw(Shader* shader);
	void DrawSuns(Shader* shader);

private:
	struct _orbit_
	{
		float dist;
		float speed;
		Planet* planet;
		std::vector<Planet*>moons;
		void draw(Shader* shader)
		{
			//if(playSpeed > 0)
				planet->SetPos({ dist * cos(glm::radians(Global::time * speed)), 0, dist * sin(glm::radians(Global::time * speed)) });
			planet->Draw(shader);
			for (auto& it : moons)
			{
				glm::vec3 pos = planet->GetPos();
				//if (playSpeed > 0)
					it->SetPos(pos +  0.3f * glm::vec3{ cos(glm::radians(Global::time * 30)), 0.2f, sin(glm::radians(Global::time * 30)) });
				it->Draw(shader);
			}
		};
	};
public:
	std::vector<_orbit_*>orbits;
	std::vector<Planet*>suns;
};