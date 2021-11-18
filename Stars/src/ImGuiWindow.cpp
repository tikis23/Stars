#include "ImGuiWindow.h"
void ImGuiWindow::Setup(Window* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window->GetHandle(), true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::GetIO().IniFilename = NULL;
}

void ImGuiWindow::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiWindow::StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiWindow::EndFrame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool *ImGuiWindow::Variable_bool(std::string name)
{
	return &boolVariables[name];
}

int *ImGuiWindow::Variable_int(std::string name)
{
	return &intVariables[name];
}

float *ImGuiWindow::Variable_float(std::string name)
{
	return &floatVariables[name];
}

std::string* ImGuiWindow::Variable_string(std::string name)
{
	return &stringVariables[name];
}

std::unordered_map<std::string, bool> ImGuiWindow::boolVariables;
std::unordered_map<std::string, int> ImGuiWindow::intVariables;
std::unordered_map<std::string, float> ImGuiWindow::floatVariables;
std::unordered_map<std::string, std::string> ImGuiWindow::stringVariables;