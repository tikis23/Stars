#pragma once
#include "Window.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_stdlib.h"

#include <unordered_map>
#include <string>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>


class ImGuiWindow
{
public:
	static void Setup(Window* window);
	static void Destroy();
	static void StartFrame();
	static void EndFrame();
	static bool* Variable_bool(std::string name);
	static int* Variable_int(std::string name);
	static float* Variable_float(std::string name);
	static std::string* Variable_string(std::string name);
private:
	static std::unordered_map<std::string, bool> boolVariables;
	static std::unordered_map<std::string, int> intVariables;
	static std::unordered_map<std::string, float> floatVariables;
	static std::unordered_map<std::string, std::string> stringVariables;
	//ImGuiWindow();
};