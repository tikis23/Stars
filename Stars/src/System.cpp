#include "System.h"
#include <random>

#include "ImGuiWindow.h"


struct _star_
{
	float radius;
	glm::vec3 color;
};

System::System(bool sun)
{
	std::random_device rd;
	std::mt19937 e{ rd() };
	std::uniform_real_distribution<float> dist{ 0.2f, 1.0f };

	// sun types
	_star_ starTypes[] = {
		{dist(e),      glm::vec3{1.2f, 1.2f, 1.2f}}, // white
		{dist(e) * 4,  glm::vec3{1.5f, 1.2f, 0}}, // orange
		{dist(e) * 30, glm::vec3{0.5f, 1.2f, 1.5f}}, // blue
		{dist(e) * 60, glm::vec3{1.2f, 0.f, 0}} // red
	};

	if (sun)
	{
		// saule
		glm::vec3 rotDir = (dist(e) * 0.5f + 1.f) * glm::normalize(glm::vec3{ dist(e) * 2 - 1, dist(e) * 2 - 1, dist(e) * 2 - 1 });
		suns.emplace_back(new Planet(20.f, 0, 1000, starTypes[1].color, rotDir, {}, 0));


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
		orbits[2]->planet->variables.min_height = 0.54f;
		orbits[2]->planet->variables.max_height = 0.64f;
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
	else
	{
		//// STARS
		//dist = std::uniform_real_distribution<float>{ 0, 5 };

		//std::uniform_int_distribution<int> starType{ 0, 3 };

		//_star_ star = starTypes[starType(e)];
		//suns.emplace_back(new Planet(20.f, 0, 1000, starTypes[0].color, (dist(e) * 0.5f + 1.f) * glm::normalize(glm::vec3{ dist(e) * 2 - 1, dist(e) * 2 - 1, dist(e) * 2 - 1 }), (dist(e) * 0.5f + 2.f) * glm::normalize(glm::vec3{ dist(e) * 2 - 1, dist(e) * 2 - 1, dist(e) * 2 - 1 }), 0));
		//suns.back()->SetPos({ 0, 0, 0 });
		//suns.emplace_back(new Planet(20.f, 0, 1000, starTypes[1].color, (dist(e) * 0.5f + 1.f) * glm::normalize(glm::vec3{ dist(e) * 2 - 1, dist(e) * 2 - 1, dist(e) * 2 - 1 }), (dist(e) * 0.5f + 2.f) * glm::normalize(glm::vec3{ dist(e) * 2 - 1, dist(e) * 2 - 1, dist(e) * 2 - 1 }), 0));
		//suns.back()->SetPos({ 100, 0, 0 });
		//suns.emplace_back(new Planet(20.f, 0, 1000, starTypes[2].color, (dist(e) * 0.5f + 1.f) * glm::normalize(glm::vec3{ dist(e) * 2 - 1, dist(e) * 2 - 1, dist(e) * 2 - 1 }), (dist(e) * 0.5f + 2.f) * glm::normalize(glm::vec3{ dist(e) * 2 - 1, dist(e) * 2 - 1, dist(e) * 2 - 1 }), 0));
		//suns.back()->SetPos({ 200, 0, 0 });
		//suns.emplace_back(new Planet(20.f, 0, 1000, starTypes[3].color, (dist(e) * 0.5f + 1.f) * glm::normalize(glm::vec3{ dist(e) * 2 - 1, dist(e) * 2 - 1, dist(e) * 2 - 1 }), (dist(e) * 0.5f + 2.f) * glm::normalize(glm::vec3{ dist(e) * 2 - 1, dist(e) * 2 - 1, dist(e) * 2 - 1 }), 0));
		//suns.back()->SetPos({ 300, 0, 0 });
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
}

void System::Draw(Shader* shader)
{
	for (auto& it : orbits)
		it->draw(shader);
}

void System::DrawSuns(Shader* shader)
{
	for (auto& it : suns)
		it->Draw(shader);
}

Planet* System::Creator(Player* player)
{
	Planet* output = nullptr;
	ImGui::SetNextWindowSize({384, 490});
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
							output = suns[i];
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
					suns.emplace_back(new Planet(20.f, 0, 1000, glm::vec3{ 1.5f, 1.2f, 0 }, { 1, 0, 0 }, {}, 0));
			}

			ImGui::EndChild();
		}
		if (ImGui::BeginChild("Settings", { 368, 300 }, true, ImGuiWindowFlags_NoScrollbar))
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
						output = suns[*ImGuiWindow::Variable_int("PlanetSelectedID")];
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
					it = suns[*ImGuiWindow::Variable_int("PlanetSelectedID")];
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
				if (ImGui::DragFloat4("Color", glm::value_ptr(it->variables.color), 0.01f, 0.001f));
				if (ImGui::DragFloat3("Rotation", glm::value_ptr(it->variables.planetRot), 0.01f, 0.001f));
				if (ImGui::DragFloat4("Ring Color", glm::value_ptr(it->variables.colorRing), 0.01f, 0.001f));
				if (ImGui::DragFloat3("Ring Rotation", glm::value_ptr(it->variables.ringRot), 0.01f, 0.001f));
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