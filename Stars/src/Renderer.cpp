#include "Renderer.h"
#include "Input.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include "ImGuiWindow.h"

bool Checkmark(int num1, int num2)
{
	if (num1 - num2 == 0)
		return true;
	return false;
}

Renderer::Renderer(Window* window)
{
	this->window = window;

	shader = new Shader;
	shader->LoadShaders("Main", "res/shaders/main.glsl.vs", "res/shaders/main.glsl.fs", "res/shaders/main.glsl.gs");
	compute = new Shader;
	compute->LoadComputeShader("Compute", "res/shaders/bloom.glsl.cs");
	threshold = new Shader;
	threshold->LoadComputeShader("Threshold", "res/shaders/threshold.glsl.cs");
	orbitShader = new Shader;
	orbitShader->LoadShaders("orbitShader", "res/shaders/orbit.glsl.vs", "res/shaders/orbit.glsl.fs");


	player = new Player(window);
	player->camera->SetPosition({ 0, 0, -50 });
	skybox = new Skybox;

	system = new System{ true };

	int width, height;
	window->GetSize(&width, &height);
	glGenTextures(1, &output);
	glBindTexture(GL_TEXTURE_2D, output);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, Global::lod);
	glGenerateMipmap(GL_TEXTURE_2D);
}

Renderer::~Renderer()
{
	delete system;
	delete player;
}

void Renderer::Draw(Shader* fboDrawShader, unsigned int fbo, unsigned int fbovao, unsigned int fboTexture)
{
	// IMGUI
	if (ImGui::Begin("Main", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_MenuBar |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove|
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground))
	{
		
		ImGui::BeginMenuBar();
		if (ImGui::BeginMenu("Scene"))
		{
			if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
			if (ImGui::BeginMenu("New..", "Ctrl+N")) 
			{
				if (ImGui::MenuItem("Generated")) { delete system; system = new System{ false }; }
				if (ImGui::MenuItem("Empty")) { delete system; system = new System{ false }; }
				if (ImGui::MenuItem("Home")) { delete system; system = new System{ true }; }
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Settings"))
		{
			if (ImGui::BeginMenu("Time Speed"))
			{
				ImGui::SetNextItemWidth(150);
				if (ImGui::DragFloat("", &Global::timeSpeed, 0.25f));
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Bloom"))
			{
				if (ImGui::DragInt("LOD", &Global::lod, 0.25f));
				if (ImGui::DragInt("Blur Passes", &Global::passes, 0.25f));
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Controls"))
			{
				if (ImGui::DragFloat("Speed", &player->speed, 0.25f));
				if (ImGui::DragFloat("Sensitivity", &player->sensitivity, 0.25f));
				if (ImGui::DragFloat("Scroll Sensitivity", &player->scrollSensitivity, 0.025f));
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Draw Orbits", "", ImGuiWindow::Variable_bool("drawOrbits")));
			if (ImGui::DragInt("Smooth Animation", &Global::maxFrames));
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Camera"))
		{
			if (ImGui::MenuItem("Freecam", "", ImGuiWindow::Variable_bool("Freecam")))
			{
				if (*ImGuiWindow::Variable_bool("Freecam") == false)
				{
					*ImGuiWindow::Variable_int("Orbitcam") = 0;
					*ImGuiWindow::Variable_int("SmoothMove") = Global::maxFrames;
					orbitPlanet = nullptr;
					player->orbitPos = { 0, 0, 0 };
					player->orbitDist = 50;
					player->orbitSpeed = player->orbitDist / 50;
					player->orbitMin = 0;
				}
				else
				{
					orbitPlanet = nullptr;
					*ImGuiWindow::Variable_int("Orbitcam") = -1;
					glm::vec3 pos = glm::normalize(player->orbitPos - player->camera->GetPosition());
					float y = -glm::degrees(atan2(pos.x, pos.z));
					float p = glm::degrees(asin(pos.y));
					y += 90;
					if (y < 0)
						y += 360;
					player->camera->SetRotation(y, p);
				}
			}
			if (ImGui::BeginMenu("Orbitcam"))
			{
				int currentItemIndex = 0;
				if (ImGui::MenuItem("Origin", "", Checkmark(currentItemIndex, *ImGuiWindow::Variable_int("Orbitcam"))))
				{
					*ImGuiWindow::Variable_int("Orbitcam") = currentItemIndex;
					*ImGuiWindow::Variable_bool("Freecam") = 0;
					*ImGuiWindow::Variable_int("SmoothMove") = Global::maxFrames;
					orbitPlanet = nullptr;
					player->orbitPos = { 0, 0, 0 };
					player->orbitDist = 50;
					player->orbitSpeed = player->orbitDist / 50;
					player->orbitMin = 0;
				}
				currentItemIndex = 1;
				if (ImGui::BeginMenu("Suns"))
				{
					for (int suns = 0; suns < system->suns.size(); suns++)
					{
						if (ImGui::MenuItem(("Sun " + std::to_string(suns + 1)).c_str(), "", Checkmark(currentItemIndex, *ImGuiWindow::Variable_int("Orbitcam"))))
						{
							*ImGuiWindow::Variable_int("Orbitcam") = currentItemIndex;
							*ImGuiWindow::Variable_bool("Freecam") = 0;
							*ImGuiWindow::Variable_int("SmoothMove") = Global::maxFrames;
							orbitPlanet = system->suns[suns];
							player->orbitDist = orbitPlanet->size * 2;
							player->orbitSpeed = player->orbitDist / 50;
							player->orbitMin = orbitPlanet->size * 1.1f;
						}
						currentItemIndex++;
					}
					ImGui::EndMenu();
				}
				currentItemIndex = 1 + system->suns.size();
				for (int orbits = 0; orbits < system->orbits.size(); orbits++)
				{
					if (ImGui::BeginMenu(("Orbit " + std::to_string(orbits + 1)).c_str()))
					{
						if (ImGui::MenuItem("Planet", "", Checkmark(currentItemIndex, *ImGuiWindow::Variable_int("Orbitcam"))))
						{
							*ImGuiWindow::Variable_int("Orbitcam") = currentItemIndex;
							*ImGuiWindow::Variable_bool("Freecam") = 0;
							*ImGuiWindow::Variable_int("SmoothMove") = Global::maxFrames;
							orbitPlanet = system->orbits[orbits]->planet;
							player->orbitDist = orbitPlanet->size * 2;
							player->orbitSpeed = player->orbitDist / 50;
							player->orbitMin = orbitPlanet->size * 1.1f;
						}
						for (int moons = 0; moons < system->orbits[orbits]->moons.size(); moons++)
						{
							if (ImGui::MenuItem(("Moon " + std::to_string(moons + 1)).c_str(), "", Checkmark(currentItemIndex + moons + 1, *ImGuiWindow::Variable_int("Orbitcam"))))
							{
								*ImGuiWindow::Variable_int("Orbitcam") = currentItemIndex + moons + 1;
								*ImGuiWindow::Variable_bool("Freecam") = 0;
								*ImGuiWindow::Variable_int("SmoothMove") = Global::maxFrames;
								orbitPlanet = system->orbits[orbits]->moons[moons];
								player->orbitDist = orbitPlanet->size * 2;
								player->orbitSpeed = player->orbitDist / 50;
								player->orbitMin = orbitPlanet->size * 1.1f;
							}
						}
						ImGui::EndMenu();
					}
					currentItemIndex += system->orbits[orbits]->moons.size() + 1;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::Button("Creator"))
		{ 
			*ImGuiWindow::Variable_bool("ClosedCreator") = true;
			*ImGuiWindow::Variable_bool("Creator") = true; 
		}
		ImGui::EndMenuBar();
		ImGui::End();
	}
	if (*ImGuiWindow::Variable_bool("Creator") == true)
	{
		Planet* temp = system->Creator(player);
		if (temp != nullptr)
			orbitPlanet = temp;
	}




	system->Update();

	// input
	if(orbitPlanet != nullptr)
		player->orbitPos = orbitPlanet->GetPos();
	else
		player->orbitPos = { 0, 0, 0 };
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	player->SmoothMove();

	player->UpdateInput();

	// drawing
	glEnable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader->Use();
	int width, height;
	window->GetSize(&width, &height);
	shader->setUniform3f("u_viewPos", player->camera->GetPosition().x, player->camera->GetPosition().y, player->camera->GetPosition().z);
	shader->setUniformMatrix4fv("projection", glm::value_ptr(player->camera->GetProjMat()));
	shader->setUniformMatrix4fv("view", glm::value_ptr(player->camera->GetViewMat()));
	system->DrawSuns(shader);
	system->Draw(shader);
	if (*ImGuiWindow::Variable_bool("drawOrbits"))
	{
		glDisable(GL_CULL_FACE);
		orbitShader->Use();
		orbitShader->setUniform4f("u_color", 1, 1, 1, 1);
		orbitShader->setUniformMatrix4fv("projection", glm::value_ptr(player->camera->GetProjMat()));
		orbitShader->setUniformMatrix4fv("view", glm::value_ptr(player->camera->GetViewMat()));
		
		for (auto& it : system->orbits)
		{
			if(it->planet != nullptr)
				it->planet->DrawOrbit(orbitShader);
			for (auto& moons : it->moons)
				moons->DrawOrbit(orbitShader);
		}
		glEnable(GL_CULL_FACE);
		shader->Use();
	}
	Blur(fboTexture);
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	skybox->Draw(player->camera);
	fboDrawShader->Use();
	glBindVertexArray(fbovao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fboTexture);
	fboDrawShader->setUniform1i("renderedTexture", 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, output);
	fboDrawShader->setUniform1i("bloomTexture", 1);
	fboDrawShader->setUniform1i("bloomLODs", Global::lod);
	glDrawArrays(GL_TRIANGLES, 0, 6);


}

void Renderer::Blur(unsigned int texture)
{
	int width, height;
	window->GetSize(&width, &height);
	threshold->Use();

	glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA16F);
	glBindImageTexture(1, output, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);
	glDispatchCompute((GLuint)width / 16, (GLuint)height / 8, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	glGenerateTextureMipmap(output);
	compute->Use();
	for (int d = 0; d < Global::passes; d++)
	{
		for (int i = 1; i < Global::lod; i++)
		{
			GLuint workGroup_x = (GLuint)width / pow(2, i) / 4;
			GLuint workGroup_y = (GLuint)height / pow(2, i) / 2;

			glBindImageTexture(0, output, i, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA16F);
			compute->setUniform2i("dir", 0, 1);
			glDispatchCompute(workGroup_x, workGroup_y, 1);
			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
			compute->setUniform2i("dir", 1, 0);
			glDispatchCompute(workGroup_x, workGroup_y, 1);
			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		}
	}
}