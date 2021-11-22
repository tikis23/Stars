#include "System.h"
#include <random>

#include "ImGuiWindow.h"

System::System(int mode)
{
	std::random_device rd;
	std::mt19937 e{ rd() };
	std::uniform_real_distribution<float> dist{ 0.2f, 1.0f };

	// sun types
	glm::vec3 starColors[] = {
		glm::vec3{1.2f, 1.2f, 1.2f}, // white
		glm::vec3{1.5f, 1.2f, 0}, // orange
		glm::vec3{0.5f, 1.2f, 1.5f}, // blue
		glm::vec3{1.2f, 0.f, 0} // red
	};

	if (mode == 1)
	{
		// saule
		glm::vec3 rotDir = (dist(e) * 0.5f + 1.f) * glm::normalize(glm::vec3{ dist(e) * 2 - 1, dist(e) * 2 - 1, dist(e) * 2 - 1 });
		suns.emplace_back(new _orbit_);
		suns[0]->planet = new Planet(20.f, 0, 1000, starColors[1], rotDir, {}, 0);


		float padding = 20;
		// merkurijus
		rotDir = 0.5f * (dist(e) * 0.5f + 1.f) * glm::normalize(glm::vec3{ dist(e) * 2 - 1, dist(e) * 2 - 1, dist(e) * 2 - 1 });
		orbits.emplace_back(new _orbit_);
		orbits[0]->planet = new Planet(0.049f, 0, 450, { 0.69, 0.57, 0.26 }, rotDir, {}, 1);
		orbits[0]->planet->variables.speed = 4.14f;
		orbits[0]->planet->variables.dist = padding + 6.f;
		orbits[0]->planet->variables.origin = 360 * dist(e);

		// venera
		rotDir = 0.5f * (dist(e) * 0.5f + 1.f) * glm::normalize(glm::vec3{ dist(e) * 2 - 1, dist(e) * 2 - 1, dist(e) * 2 - 1 });
		orbits.emplace_back(new _orbit_);
		orbits[1]->planet = new Planet(0.121f, 0, 800, { 0.6, 0.4, 0.2 }, rotDir, {}, 1);
		orbits[1]->planet->variables.speed = 1.62f;
		orbits[1]->planet->variables.dist = padding + 8.f;
		orbits[1]->planet->variables.origin = 360 * dist(e);

		// zeme
		rotDir = 0.4f * (dist(e) * 0.5f + 1.f) * glm::normalize(glm::vec3{ dist(e) * 2 - 1, dist(e) * 2 - 1, dist(e) * 2 - 1 });
		orbits.emplace_back(new _orbit_);
		orbits[2]->planet = new Planet(0.127f, 5, 800, { 0.0, 1, 0.7 }, rotDir, {}, 1);
		orbits[2]->planet->variables.speed = 1;
		orbits[2]->planet->variables.dist = padding + 10.f;
		orbits[2]->planet->variables.min_height = 0.49f;
		orbits[2]->planet->variables.max_height = 0.66f;
		orbits[2]->moons.emplace_back(new Planet(0.027f, 0, 850, { 0.4, 0.4, 0.4 }, { 0.5, -0.2, 0.3 }, {}, 3));
		orbits[2]->planet->variables.origin = 360 * dist(e);

		// marsas
		rotDir = 0.35f * (dist(e) * 0.5f + 1.f) * glm::normalize(glm::vec3{ dist(e) * 2 - 1, dist(e) * 2 - 1, dist(e) * 2 - 1 });
		orbits.emplace_back(new _orbit_);
		orbits[3]->planet = new Planet(0.068f, 0, 600, { 0.6, 0.2, 0.2 }, rotDir, {}, 1);
		orbits[3]->planet->variables.speed = 0.53f;
		orbits[3]->planet->variables.dist = padding + 12.f;
		orbits[3]->planet->variables.min_height = 0.48f;
		orbits[3]->planet->variables.max_height = 1.f;
		orbits[3]->planet->variables.origin = 360 * dist(e);

		// jupiteris
		rotDir = 0.3f * (dist(e) * 0.5f + 1.f) * glm::normalize(glm::vec3{ dist(e) * 2 - 1, dist(e) * 2 - 1, dist(e) * 2 - 1 });
		orbits.emplace_back(new _orbit_);
		orbits[4]->planet = new Planet(1.43f, 8, 8000, { 0.9, 0.6, 0.3 }, rotDir, {}, 2);
		orbits[4]->planet->variables.speed = 0.083f;
		orbits[4]->planet->variables.dist = padding + 15.f;
		orbits[4]->planet->variables.origin = 360 * dist(e);

		// saturnas
		rotDir = 0.3f * (dist(e) * 0.5f + 1.f) * glm::normalize(glm::vec3{ dist(e) * 2 - 1, dist(e) * 2 - 1, dist(e) * 2 - 1 });
		orbits.emplace_back(new _orbit_);
		orbits[5]->planet = new Planet(1.20f, 0, 600, { 0.8, 0.8, 0.4 }, rotDir, rotDir, 2, { 0.6, 0.6, 0.2 }, true);
		orbits[5]->planet->variables.speed = 0.03f;
		orbits[5]->planet->variables.dist = padding + 23.f;
		orbits[5]->planet->variables.origin = 360 * dist(e);

		// uranas
		rotDir = 0.25f * (dist(e) * 0.5f + 1.f) * glm::normalize(glm::vec3{ dist(e) * 2 - 1, dist(e) * 2 - 1, dist(e) * 2 - 1 });
		orbits.emplace_back(new _orbit_);
		orbits[6]->planet = new Planet(0.51f, 0, 400, { 0.66, 0.86, 0.93 }, rotDir, {}, 2);
		orbits[6]->planet->variables.speed = 0.012f;
		orbits[6]->planet->variables.dist = padding + 28.f;
		orbits[6]->planet->variables.origin = 360 * dist(e);

		// neptunas
		rotDir = 0.25f * (dist(e) * 0.5f + 1.f) * glm::normalize(glm::vec3{ dist(e) * 2 - 1, dist(e) * 2 - 1, dist(e) * 2 - 1 });
		orbits.emplace_back(new _orbit_);
		orbits[7]->planet = new Planet(0.50f, 0, 500, { 0.4, 0.6, 0.8 }, rotDir, {}, 2);
		orbits[7]->planet->variables.speed = 0.006f;
		orbits[7]->planet->variables.dist = padding + 30.f;
		orbits[7]->planet->variables.origin = 360 * dist(e);
	}
	else if(mode == 2)
	{
		// STARS
		std::uniform_int_distribution<int> starAmountDist{ 1, 5 };
		std::uniform_int_distribution<int> starColorDist{ 0, 3 };
		std::uniform_real_distribution<float> starSizeDist{ 5, 40 };
		std::uniform_real_distribution<float> speedDist{ 1, 10 };

		int starAmount = 1;
		for(int i = 0; i < 5; i++)
			starAmount += starAmountDist(e) / 5;
		float originDegree = 360.0f / starAmount;
		float sunSize = starSizeDist(e);
		float sunDist = sunSize * (std::max((float)starAmount / 2, 0.6f)) * ((float)(starAmountDist(e) - 1) * 0.25f + 1);
		float sunSpeed = speedDist(e);
		if (starAmount == 1)
			sunDist = 0;
		for (int i = 0; i < starAmount; i++)
		{
			suns.push_back(new _orbit_);
			suns.back()->planet = new Planet(sunSize, 0, 1000, starColors[starColorDist(e)], glm::vec3{ dist(e) - 0.1f, dist(e) - 0.1f,dist(e) - 0.1f } * 2.0f, {}, 0);
			suns.back()->planet->variables.dist = sunDist;
			suns.back()->planet->variables.speed = sunSpeed;
			suns.back()->planet->variables.origin = originDegree * i;
		}

		// planets
		float padding = sunDist * 2;
		if (starAmount == 1)
			padding = sunSize * 2;
		std::uniform_int_distribution<int> planetVertexDetailDist{ 4, 7 };
		std::uniform_int_distribution<int> planetAmountDist{ 1, 20 };
		std::uniform_real_distribution<float> planetSizeDist{ 0.2f, sunSize / 3 };
		std::uniform_real_distribution<float> planetDetailDist{ 100, 1000 };
		std::uniform_real_distribution<float> colorDist{ 0, 1 };
		speedDist = std::uniform_real_distribution<float>{ 1, 5 };
		int planetAmount = planetAmountDist(e);
		for (int i = 0; i < planetAmount; i++)
		{
			float planetSize = planetSizeDist(e);
			int planetType = (int)planetDetailDist(e) % 2 + 1;

			glm::vec3 color = glm::normalize(glm::vec3{ colorDist(e), colorDist(e), colorDist(e) }) * 2.f;
			if (color.x > 1.1f) color.x = 1.1f;
			if (color.y > 1.1f) color.y = 1.1f;
			if (color.z > 1.1f) color.z = 1.1f;
			glm::vec3 ringColor = glm::normalize(glm::vec3{colorDist(e), colorDist(e), colorDist(e)}) * 2.f;
			if (ringColor.x > 1.1f) ringColor.x = 1.1f;
			if (ringColor.y > 1.1f) ringColor.y = 1.1f;
			if (ringColor.z > 1.1f) ringColor.z = 1.1f;
			orbits.push_back(new _orbit_);
			orbits.back()->planet = new Planet(planetSize, planetVertexDetailDist(e), planetDetailDist(e), color, glm::vec3{ dist(e) - 0.1f, dist(e) - 0.1f,dist(e) - 0.1f } *2.0f,
				glm::vec3{ dist(e) - 0.1f, dist(e) - 0.1f,dist(e) - 0.1f } *2.0f, planetType, ringColor, (planetDetailDist(e) < 210) ? true : false);
			padding += planetSize;
			orbits.back()->planet->variables.dist = padding;
			orbits.back()->planet->variables.speed = speedDist(e);
			orbits.back()->planet->variables.origin = colorDist(e) * 360.0f;
			color = glm::normalize(glm::vec3{ colorDist(e), colorDist(e), colorDist(e) }) * 1.5f;
			orbits.back()->planet->variables.minColor = color;
			color = glm::normalize(glm::vec3{ colorDist(e), colorDist(e), colorDist(e) }) * 1.5f;
			orbits.back()->planet->variables.maxColor = color;

			if (planetType == 1)
			{
				if (planetDetailDist(e) > 300)
					orbits.back()->planet->variables.min_height = colorDist(e) / 3 + 0.16f;
				if (planetDetailDist(e) > 300)
					orbits.back()->planet->variables.max_height = colorDist(e) / 3 + 0.66f;
			}
			padding += planetSize + planetDetailDist(e) * 0.2f;

			// moons
			if (orbits.back()->planet->variables.hasRing)
				continue;
			int moonAmount = planetAmountDist(e) / 5;
			if (orbits.back()->planet->variables.isGas)
				moonAmount = planetAmountDist(e);
			float moonOriginDegree = 360.0f / moonAmount;
			float moonPadding = planetSize * 1.5f;
			for (int j = 0; j < moonAmount; j++)
			{
				float moonSize = planetSize / 20 * (colorDist(e) + 0.5f);
				color = glm::normalize(glm::vec3{ colorDist(e), colorDist(e), colorDist(e) }) * 2.f;
				if (color.x > 1.1f) color.x = 1.1f;
				if (color.y > 1.1f) color.y = 1.1f;
				if (color.z > 1.1f) color.z = 1.1f;
				orbits.back()->moons.emplace_back(new Planet(moonSize, 4, planetDetailDist(e) , color, glm::vec3{ dist(e) - 0.1f, dist(e) - 0.1f,dist(e) - 0.1f } *2.0f,
					glm::vec3{ dist(e) - 0.1f, dist(e) - 0.1f,dist(e) - 0.1f } *2.0f, 3, {}, false));

				moonPadding += moonSize;
				orbits.back()->moons.back()->variables.dist = moonPadding;
				orbits.back()->moons.back()->variables.speed = speedDist(e);
				orbits.back()->moons.back()->variables.origin = colorDist(e) * 360.0f;
				moonPadding += moonSize * 2;
			}
			padding += moonPadding / 3;
		}
	}
	*ImGuiWindow::Variable_int("OrbitSelectedID") = -1;
	*ImGuiWindow::Variable_int("PlanetSelectedID") = -1;
}

System::~System()
{
	for (int i = 0; i < suns.size(); i++)
		delete suns[i];
	for (int i = 0; i < orbits.size(); i++)
		delete orbits[i];
}

void System::Update()
{
	for (auto& it : orbits)
		it->Update();
	for (auto& it : suns)
		it->Update();
}

void System::Draw(Shader* shader)
{
	for (auto& it : orbits)
		it->draw(shader);
}

void System::DrawSuns(Shader* shader)
{
	for (auto& it : suns)
		it->draw(shader);
}

Planet* System::Creator(Player* player)
{
	Planet* output = nullptr;
	ImGui::SetNextWindowSize({384, 538});
	if(ImGui::Begin("Creator", ImGuiWindow::Variable_bool("ClosedCreator"), ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
	{
		if (ImGui::BeginChild("Orbits", {180, 150}, true, ImGuiWindowFlags_AlwaysVerticalScrollbar))
		{
			bool sunSelected = false;
			if (*ImGuiWindow::Variable_int("OrbitSelectedID") == 0)
				sunSelected = true;
			if (ImGui::Selectable("suns", &sunSelected, ImGuiSelectableFlags_AllowItemOverlap, ImVec2(170, 20)))
			{
				*ImGuiWindow::Variable_int("OrbitSelectedID") = 0;
				*ImGuiWindow::Variable_int("PlanetSelectedID") = -1;
			}
			for (int i = 0; i < orbits.size(); i++)
			{
				ImGui::PushID(i);
				bool selected = false;
				if (*ImGuiWindow::Variable_int("OrbitSelectedID") == i + 1)
					selected = true;
				if (ImGui::Selectable(("orbit " + std::to_string(i + 1)).c_str(), &selected, ImGuiSelectableFlags_AllowItemOverlap, ImVec2(170, 20)))
				{
					*ImGuiWindow::Variable_int("OrbitSelectedID") = i + 1;
					*ImGuiWindow::Variable_int("PlanetSelectedID") = -1;
				}
				ImGui::SameLine(145);
				if (ImGui::Button("X"))
				{
					delete orbits[i];
					orbits.erase(orbits.begin() + i);
					*ImGuiWindow::Variable_int("PlanetSelectedID") = -1;
					if (*ImGuiWindow::Variable_bool("CreatorFocusPlanet") && *ImGuiWindow::Variable_int("OrbitSelectedID") == i + 1)
					{
						*ImGuiWindow::Variable_bool("Freecam") = 0;
						*ImGuiWindow::Variable_int("Orbitcam") = 0;
						*ImGuiWindow::Variable_int("SmoothMove") = Global::maxFrames;
						player->orbitPos = { 0, 0, 0 };
						player->orbitDist = 50;
						player->orbitSpeed = player->orbitDist / 50;
						player->orbitMin = 0;
					}
					if (*ImGuiWindow::Variable_int("OrbitSelectedID") == i + 1)
						*ImGuiWindow::Variable_int("OrbitSelectedID") = -1;
					else if (*ImGuiWindow::Variable_int("OrbitSelectedID") > i + 1)
						*ImGuiWindow::Variable_int("OrbitSelectedID") = *ImGuiWindow::Variable_int("OrbitSelectedID") - 1;
					i--;
				}
				ImGui::PopID();
			}
			if (ImGui::Button("Add Orbit"))
			{
				orbits.emplace_back(new _orbit_);
			}
			ImGui::EndChild();
		}
		ImGui::SameLine();
		if (ImGui::BeginChild("Planets", { 180, 150 }, true, ImGuiWindowFlags_AlwaysVerticalScrollbar))
		{
			if (*ImGuiWindow::Variable_int("OrbitSelectedID") >= 1)
			{
				// planet
				bool planetSelected = false;
				if (*ImGuiWindow::Variable_int("PlanetSelectedID") == 0)
					planetSelected = true;
				if (orbits[*ImGuiWindow::Variable_int("OrbitSelectedID") - 1]->planet != nullptr)
				{
					if (ImGui::Selectable("planet", &planetSelected, ImGuiSelectableFlags_AllowItemOverlap, ImVec2(170, 20)))
					{
						*ImGuiWindow::Variable_int("PlanetSelectedID") = 0;
						if (*ImGuiWindow::Variable_bool("CreatorFocusPlanet"))
						{
							*ImGuiWindow::Variable_int("Orbitcam") = suns.size() + *ImGuiWindow::Variable_int("OrbitSelectedID");
							*ImGuiWindow::Variable_bool("Freecam") = 0;
							*ImGuiWindow::Variable_int("SmoothMove") = Global::maxFrames;
							output = orbits[*ImGuiWindow::Variable_int("OrbitSelectedID") - 1]->planet;
							player->orbitDist = output->variables.size * 2;
							player->orbitSpeed = player->orbitDist / 50;
							player->orbitMin = output->variables.size * 1.1f;
						}
					}
					if (orbits[*ImGuiWindow::Variable_int("OrbitSelectedID") - 1]->moons.empty())
					{
						ImGui::SameLine(145);
						if (ImGui::Button("X"))
						{
							delete orbits[*ImGuiWindow::Variable_int("OrbitSelectedID") - 1]->planet;
							orbits[*ImGuiWindow::Variable_int("OrbitSelectedID") - 1]->planet = nullptr;

							if (*ImGuiWindow::Variable_bool("CreatorFocusPlanet") && *ImGuiWindow::Variable_int("PlanetSelectedID") == 0)
							{
								*ImGuiWindow::Variable_bool("Freecam") = 0;
								*ImGuiWindow::Variable_int("Orbitcam") = 0;
								*ImGuiWindow::Variable_int("SmoothMove") = Global::maxFrames;
								player->orbitPos = { 0, 0, 0 };
								player->orbitDist = 50;
								player->orbitSpeed = player->orbitDist / 50;
								player->orbitMin = 0;
							}
							if (*ImGuiWindow::Variable_int("PlanetSelectedID") == 0)
								*ImGuiWindow::Variable_int("PlanetSelectedID") = -1;
						}
					}
				}
				else if (orbits[*ImGuiWindow::Variable_int("OrbitSelectedID") - 1]->planet == nullptr)
					if (ImGui::Button("Add Planet"))
					{
						orbits[*ImGuiWindow::Variable_int("OrbitSelectedID") - 1]->planet = new Planet;
						orbits[*ImGuiWindow::Variable_int("OrbitSelectedID") - 1]->planet->variables.isPlanet = true;
						orbits[*ImGuiWindow::Variable_int("OrbitSelectedID") - 1]->planet->variables.dist = 25.f;
					}
				// moons
				for (int i = 0; i < orbits[*ImGuiWindow::Variable_int("OrbitSelectedID") - 1]->moons.size(); i++)
				{
					ImGui::PushID(i);
					bool selected = false;
					if (*ImGuiWindow::Variable_int("PlanetSelectedID") == i + 1)
						selected = true;
					if (ImGui::Selectable(("moon " + std::to_string(i + 1)).c_str(), &selected, ImGuiSelectableFlags_AllowItemOverlap, ImVec2(170, 20)))
					{
						*ImGuiWindow::Variable_int("PlanetSelectedID") = i + 1;
						if (*ImGuiWindow::Variable_bool("CreatorFocusPlanet"))
						{
							*ImGuiWindow::Variable_int("Orbitcam") = suns.size() + *ImGuiWindow::Variable_int("OrbitSelectedID") + 
								*ImGuiWindow::Variable_int("PlanetSelectedID");
							*ImGuiWindow::Variable_bool("Freecam") = 0;
							*ImGuiWindow::Variable_int("SmoothMove") = Global::maxFrames;
							output = orbits[*ImGuiWindow::Variable_int("OrbitSelectedID") - 1]->moons[i];
							player->orbitDist = output->variables.size * 2;
							player->orbitSpeed = player->orbitDist / 50;
							player->orbitMin = output->variables.size * 1.1f;
						}
					}
					ImGui::SameLine(145);
					if (ImGui::Button("X"))
					{
						delete orbits[*ImGuiWindow::Variable_int("OrbitSelectedID") - 1]->moons[i];
						orbits[*ImGuiWindow::Variable_int("OrbitSelectedID") - 1]->moons.erase(orbits[*ImGuiWindow::Variable_int("OrbitSelectedID") - 1]->moons.begin() + i);

						if (*ImGuiWindow::Variable_bool("CreatorFocusPlanet") && *ImGuiWindow::Variable_int("PlanetSelectedID") == i + 1)
						{
							*ImGuiWindow::Variable_bool("Freecam") = 0;
							*ImGuiWindow::Variable_int("Orbitcam") = 0;
							*ImGuiWindow::Variable_int("SmoothMove") = Global::maxFrames;
							player->orbitPos = { 0, 0, 0 };
							player->orbitDist = 50;
							player->orbitSpeed = player->orbitDist / 50;
							player->orbitMin = 0;
						}
						if (*ImGuiWindow::Variable_int("PlanetSelectedID") == i + 1)
							*ImGuiWindow::Variable_int("PlanetSelectedID") = -1;
						else if (*ImGuiWindow::Variable_int("PlanetSelectedID") > i + 1)
							*ImGuiWindow::Variable_int("PlanetSelectedID") = *ImGuiWindow::Variable_int("PlanetSelectedID") - 1;
						i--;
					}
					ImGui::PopID();
				}
				if (orbits[*ImGuiWindow::Variable_int("OrbitSelectedID") - 1]->planet != nullptr)
					if (ImGui::Button("Add Moon"))
					{
						orbits[*ImGuiWindow::Variable_int("OrbitSelectedID") - 1]->moons.emplace_back(new Planet);
						orbits[*ImGuiWindow::Variable_int("OrbitSelectedID") - 1]->moons.back()->variables.isMoon = true;
						orbits[*ImGuiWindow::Variable_int("OrbitSelectedID") - 1]->moons.back()->variables.size = 0.03f;
						orbits[*ImGuiWindow::Variable_int("OrbitSelectedID") - 1]->moons.back()->variables.dist = 0.2f;
					}
			}
			else if (*ImGuiWindow::Variable_int("OrbitSelectedID") == 0)
			{
				// suns
				for (int i = 0; i < suns.size(); i++)
				{
					ImGui::PushID(i);
					bool selected = false;
					if (*ImGuiWindow::Variable_int("PlanetSelectedID") == i)
						selected = true;
					if (ImGui::Selectable(("sun " + std::to_string(i + 1)).c_str(), &selected, ImGuiSelectableFlags_AllowItemOverlap, ImVec2(170, 20)))
					{
						*ImGuiWindow::Variable_int("PlanetSelectedID") = i;
						if (*ImGuiWindow::Variable_bool("CreatorFocusPlanet"))
						{
							*ImGuiWindow::Variable_int("Orbitcam") = 1 + i;
							*ImGuiWindow::Variable_bool("Freecam") = 0;
							*ImGuiWindow::Variable_int("SmoothMove") = Global::maxFrames;
							output = suns[i]->planet;
							player->orbitDist = output->variables.size * 2;
							player->orbitSpeed = player->orbitDist / 50;
							player->orbitMin = output->variables.size * 1.1f;
						}
					}
					ImGui::SameLine(145);
					if (ImGui::Button("X"))
					{
						delete suns[i];
						suns.erase(suns.begin() + i);

						if (*ImGuiWindow::Variable_bool("CreatorFocusPlanet") && *ImGuiWindow::Variable_int("PlanetSelectedID") == i)
						{
							*ImGuiWindow::Variable_bool("Freecam") = 0;
							*ImGuiWindow::Variable_int("Orbitcam") = 0;
							*ImGuiWindow::Variable_int("SmoothMove") = Global::maxFrames;
							player->orbitPos = { 0, 0, 0 };
							player->orbitDist = 50;
							player->orbitSpeed = player->orbitDist / 50;
							player->orbitMin = 0;
						}
						if (*ImGuiWindow::Variable_int("PlanetSelectedID") == i)
							*ImGuiWindow::Variable_int("PlanetSelectedID") = -1;
						else if (*ImGuiWindow::Variable_int("PlanetSelectedID") > i)
							*ImGuiWindow::Variable_int("PlanetSelectedID") = *ImGuiWindow::Variable_int("PlanetSelectedID") - 1;
						i--;
					}
					ImGui::PopID();
				}
				if (ImGui::Button("Add Sun"))
				{
					suns.emplace_back(new _orbit_);
					suns.back()->planet = new Planet(20.f, 0, 1000, glm::vec3{ 1.5f, 1.2f, 0 }, { 1, 0, 0 }, {}, 0);
				}
			}

			ImGui::EndChild();
		}
		if (ImGui::BeginChild("Settings", { 368, 348 }, true, ImGuiWindowFlags_NoScrollbar))
		{
			if (ImGui::Checkbox("Focus", ImGuiWindow::Variable_bool("CreatorFocusPlanet")))
			{
				if (*ImGuiWindow::Variable_bool("CreatorFocusPlanet") && *ImGuiWindow::Variable_int("PlanetSelectedID") >= 0)
				{
					if (*ImGuiWindow::Variable_int("OrbitSelectedID") == 0)
					{
						*ImGuiWindow::Variable_int("Orbitcam") = 1 + *ImGuiWindow::Variable_int("PlanetSelectedID");
						*ImGuiWindow::Variable_bool("Freecam") = 0;
						*ImGuiWindow::Variable_int("SmoothMove") = Global::maxFrames;
						output = suns[*ImGuiWindow::Variable_int("PlanetSelectedID")]->planet;
						player->orbitDist = output->variables.size * 2;
						player->orbitSpeed = player->orbitDist / 50;
						player->orbitMin = output->variables.size * 1.1f;
					}
					else if (*ImGuiWindow::Variable_int("OrbitSelectedID") >= 1)
					{
						if (orbits[*ImGuiWindow::Variable_int("OrbitSelectedID") - 1]->planet != nullptr && *ImGuiWindow::Variable_int("PlanetSelectedID") == 0)
						{
							*ImGuiWindow::Variable_int("Orbitcam") = suns.size() + *ImGuiWindow::Variable_int("OrbitSelectedID");
							*ImGuiWindow::Variable_bool("Freecam") = 0;
							*ImGuiWindow::Variable_int("SmoothMove") = Global::maxFrames;
							output = orbits[*ImGuiWindow::Variable_int("OrbitSelectedID") - 1]->planet;
							player->orbitDist = output->variables.size * 2;
							player->orbitSpeed = player->orbitDist / 50;
							player->orbitMin = output->variables.size * 1.1f;
						}
						else
						{
							*ImGuiWindow::Variable_int("Orbitcam") = suns.size() + *ImGuiWindow::Variable_int("OrbitSelectedID") +
								*ImGuiWindow::Variable_int("PlanetSelectedID");
							*ImGuiWindow::Variable_bool("Freecam") = 0;
							*ImGuiWindow::Variable_int("SmoothMove") = Global::maxFrames;
							output = orbits[*ImGuiWindow::Variable_int("OrbitSelectedID") - 1]->moons[*ImGuiWindow::Variable_int("PlanetSelectedID")-1];
							player->orbitDist = output->variables.size * 2;
							player->orbitSpeed = player->orbitDist / 50;
							player->orbitMin = output->variables.size * 1.1f;
						}
					}
				}
			}
			if (*ImGuiWindow::Variable_int("OrbitSelectedID") >= 0 && *ImGuiWindow::Variable_int("PlanetSelectedID") >= 0)
			{
				Planet* it;
				if (*ImGuiWindow::Variable_int("OrbitSelectedID") == 0)
					it = suns[*ImGuiWindow::Variable_int("PlanetSelectedID")]->planet;
				else if (*ImGuiWindow::Variable_int("PlanetSelectedID") == 0)
					it = orbits[*ImGuiWindow::Variable_int("OrbitSelectedID") - 1]->planet;
				else
					it = orbits[*ImGuiWindow::Variable_int("OrbitSelectedID") - 1]->moons[*ImGuiWindow::Variable_int("PlanetSelectedID") - 1];
				// settings
				ImGui::SameLine();
				if (ImGui::Button("Re-Generate"))
					it->Regenerate();
				if (ImGui::BeginChild("ShortSettings", { 352, 170 }, true, ImGuiWindowFlags_NoScrollbar))
				{
					ImGui::Columns(2, "1");
					ImGui::SetColumnWidth(0, 176);
					ImGui::PushItemWidth(80);
					if (ImGui::DragFloat("Size", &it->variables.size, 0.01f));
					if (ImGui::DragInt("Detail", &it->variables.detail, 0.01f, 0, 8));
					if (ImGui::DragFloat("Noisy", &it->variables.mult, 1.f));
					if (ImGui::DragFloat("Max Height", &it->variables.max_height, 0.01f));
					if (ImGui::DragFloat("Min Height", &it->variables.min_height, 0.01f));
					if (ImGui::DragFloat("Origin", &it->variables.origin));
					if (ImGui::DragFloat("Distance", &it->variables.dist, 0.01f));
					ImGui::PopItemWidth();
					ImGui::NextColumn();
					ImGui::PushItemWidth(80);
					if (ImGui::Checkbox("Sun", &it->variables.isSun));
					if (ImGui::Checkbox("Planet", &it->variables.isPlanet));
					if (ImGui::Checkbox("Gas", &it->variables.isGas));
					if (ImGui::Checkbox("Moon", &it->variables.isMoon));
					if (ImGui::Checkbox("Ring", &it->variables.hasRing));
					if (ImGui::DragFloat("Ring Size", &it->variables.ringSize, 0.01f));
					if (ImGui::DragFloat("Speed", &it->variables.speed, 0.01f));
					ImGui::PopItemWidth();
					ImGui::Columns();

					ImGui::EndChild();
				}
				if (ImGui::DragFloat3("Rotation", glm::value_ptr(it->variables.planetRot), 0.01f, 0));
				if (ImGui::DragFloat3("Ring Rotation", glm::value_ptr(it->variables.ringRot), 0.01f, 0));
				if (ImGui::DragFloat4("Color", glm::value_ptr(it->variables.color), 0.01f, 0));
				if (ImGui::DragFloat4("Ring Color", glm::value_ptr(it->variables.colorRing), 0.01f, 0));
				if (ImGui::DragFloat3("Max Height Color", glm::value_ptr(it->variables.maxColor), 0.01f, 0));
				if (ImGui::DragFloat3("Min Height Color", glm::value_ptr(it->variables.minColor), 0.01f, 0));
			}
			ImGui::EndChild();
		}
		ImGui::End();
	}
	if (*ImGuiWindow::Variable_bool("ClosedCreator") == false)
		*ImGuiWindow::Variable_bool("Creator") = false;
	if (!*ImGuiWindow::Variable_bool("CreatorFocusPlanet"))
		output = nullptr;

	return output;
}