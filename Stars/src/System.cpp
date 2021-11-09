#include "System.h"
#include <random>

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
		suns.emplace_back(new Planet(20.f, 1000, starTypes[1].color, rotDir, {}, 0));


		float padding = 20;
		// merkurijus
		rotDir = 0.5f * (dist(e) * 0.5f + 1.f) * glm::normalize(glm::vec3{ dist(e) * 2 - 1, dist(e) * 2 - 1, dist(e) * 2 - 1 });
		orbits.emplace_back(new _orbit_);
		orbits[0]->speed = 4.14f;
		orbits[0]->dist = padding + 6.f;
		orbits[0]->planet = new Planet(0.049f, 300, { 0.69, 0.57, 0.26 }, rotDir, {}, 1);

		// venera
		rotDir = 0.5f * (dist(e) * 0.5f + 1.f) * glm::normalize(glm::vec3{ dist(e) * 2 - 1, dist(e) * 2 - 1, dist(e) * 2 - 1 });
		orbits.emplace_back(new _orbit_);
		orbits[1]->speed = 1.62f;
		orbits[1]->dist = padding + 8.f;
		orbits[1]->planet = new Planet(0.121f, 800, { 0.6, 0.4, 0.2 }, rotDir, {}, 1);

		// zeme
		rotDir = 0.4f * (dist(e) * 0.5f + 1.f) * glm::normalize(glm::vec3{ dist(e) * 2 - 1, dist(e) * 2 - 1, dist(e) * 2 - 1 });
		orbits.emplace_back(new _orbit_);
		orbits[2]->speed = 1;
		orbits[2]->dist = padding + 10.f;
		orbits[2]->planet = new Planet(0.127f, 800, { 0.2, 0.8, 0.4 }, rotDir, {}, 1);
		orbits[2]->planet->min_height = 0.54f;
		orbits[2]->planet->max_height = 0.64f;
		orbits[2]->moons.emplace_back(new Planet(0.027f, 800, { 0.4, 0.4, 0.4 }, { 0.5, -0.2, 0.3 }, {}, 3));

		// marsas
		rotDir = 0.35f * (dist(e) * 0.5f + 1.f) * glm::normalize(glm::vec3{ dist(e) * 2 - 1, dist(e) * 2 - 1, dist(e) * 2 - 1 });
		orbits.emplace_back(new _orbit_);
		orbits[3]->speed = 0.53f;
		orbits[3]->dist = padding + 12.f;
		orbits[3]->planet = new Planet(0.068f, 600, { 0.6, 0.2, 0.2 }, rotDir, {}, 1);
		orbits[3]->planet->min_height = 0.48f;
		orbits[3]->planet->max_height = 1.f;

		// jupiteris
		rotDir = 0.3f * (dist(e) * 0.5f + 1.f) * glm::normalize(glm::vec3{ dist(e) * 2 - 1, dist(e) * 2 - 1, dist(e) * 2 - 1 });
		orbits.emplace_back(new _orbit_);
		orbits[4]->speed = 0.083f;
		orbits[4]->dist = padding + 15.f;
		orbits[4]->planet = new Planet(1.43f, 8000, { 0.9, 0.6, 0.3 }, rotDir, {}, 2);

		// saturnas
		rotDir = 0.3f * (dist(e) * 0.5f + 1.f) * glm::normalize(glm::vec3{ dist(e) * 2 - 1, dist(e) * 2 - 1, dist(e) * 2 - 1 });
		orbits.emplace_back(new _orbit_);
		orbits[5]->speed = 0.03f;
		orbits[5]->dist = padding + 23.f;
		orbits[5]->planet = new Planet(1.20f, 600, { 0.8, 0.8, 0.4 }, rotDir, rotDir, 2, { 0.6, 0.6, 0.2 }, true);

		// uranas
		rotDir = 0.25f * (dist(e) * 0.5f + 1.f) * glm::normalize(glm::vec3{ dist(e) * 2 - 1, dist(e) * 2 - 1, dist(e) * 2 - 1 });
		orbits.emplace_back(new _orbit_);
		orbits[6]->speed = 0.012f;
		orbits[6]->dist = padding + 28.f;
		orbits[6]->planet = new Planet(0.51f, 200, { 0.66, 0.86, 0.93 }, rotDir, {}, 2);

		// neptunas
		rotDir = 0.25f * (dist(e) * 0.5f + 1.f) * glm::normalize(glm::vec3{ dist(e) * 2 - 1, dist(e) * 2 - 1, dist(e) * 2 - 1 });
		orbits.emplace_back(new _orbit_);
		orbits[7]->speed = 0.006f;
		orbits[7]->dist = padding + 30.f;
		orbits[7]->planet = new Planet(0.50f, 200, { 0.4, 0.6, 0.8 }, rotDir, {}, 2);
	}
	else
	{
		// STARS
		dist = std::uniform_real_distribution<float>{ 0, 5 };

		std::uniform_int_distribution<int> starType{ 0, 3 };

		_star_ star = starTypes[starType(e)];
		suns.emplace_back(new Planet(20.f, 1000, starTypes[0].color, (dist(e) * 0.5f + 1.f) * glm::normalize(glm::vec3{ dist(e) * 2 - 1, dist(e) * 2 - 1, dist(e) * 2 - 1 }), (dist(e) * 0.5f + 2.f) * glm::normalize(glm::vec3{ dist(e) * 2 - 1, dist(e) * 2 - 1, dist(e) * 2 - 1 }), 0));
		suns.back()->SetPos({ 0, 0, 0 });
		suns.emplace_back(new Planet(20.f, 1000, starTypes[1].color, (dist(e) * 0.5f + 1.f) * glm::normalize(glm::vec3{ dist(e) * 2 - 1, dist(e) * 2 - 1, dist(e) * 2 - 1 }), (dist(e) * 0.5f + 2.f) * glm::normalize(glm::vec3{ dist(e) * 2 - 1, dist(e) * 2 - 1, dist(e) * 2 - 1 }), 0));
		suns.back()->SetPos({ 100, 0, 0 });
		suns.emplace_back(new Planet(20.f, 1000, starTypes[2].color, (dist(e) * 0.5f + 1.f) * glm::normalize(glm::vec3{ dist(e) * 2 - 1, dist(e) * 2 - 1, dist(e) * 2 - 1 }), (dist(e) * 0.5f + 2.f) * glm::normalize(glm::vec3{ dist(e) * 2 - 1, dist(e) * 2 - 1, dist(e) * 2 - 1 }), 0));
		suns.back()->SetPos({ 200, 0, 0 });
		suns.emplace_back(new Planet(20.f, 1000, starTypes[3].color, (dist(e) * 0.5f + 1.f) * glm::normalize(glm::vec3{ dist(e) * 2 - 1, dist(e) * 2 - 1, dist(e) * 2 - 1 }), (dist(e) * 0.5f + 2.f) * glm::normalize(glm::vec3{ dist(e) * 2 - 1, dist(e) * 2 - 1, dist(e) * 2 - 1 }), 0));
		suns.back()->SetPos({ 300, 0, 0 });
	}
}

System::~System()
{

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
