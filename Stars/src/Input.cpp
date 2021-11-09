#include "Input.h"
#include <GLFW/glfw3.h>

#include <vector>
#include <map>

namespace Input
{
	// private
	struct ButtonState
	{
		bool press = false;
		bool release = false;
		bool hold = false;
	};
	struct WindowState
	{
		std::map<int, ButtonState> keys;
		std::map<int, ButtonState> buttons;
		float scrollOffsetX = 0;
		float scrollOffsetY = 0;
		GLFWwindow* window;
	};
	static std::vector<WindowState>windows;
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		int index = -1;
		for (int i = 0; i < windows.size(); i++)
			if (windows[i].window == window)
			{
				index = i;
				break;
			}
		if (index > windows.size() || index < 0)
			return;
		if (action == GLFW_PRESS)
		{
			windows[index].keys[key].press = true;
			windows[index].keys[key].hold = true;
		}
		if (action == GLFW_RELEASE)
		{
			windows[index].keys[key].release = true;
			windows[index].keys[key].hold = false;
		}
	}
	static void ButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		int index = -1;
		for (int i = 0; i < windows.size(); i++)
			if (windows[i].window == window)
			{
				index = i;
				break;
			}
		if (index > windows.size() || index < 0)
			return;
		if (action == GLFW_PRESS)
		{

			windows[index].buttons[button].press = true;
			windows[index].buttons[button].hold = true;
		}
		if (action == GLFW_RELEASE)
		{
			windows[index].buttons[button].release = true;
			windows[index].buttons[button].hold = false;
		}
	}
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		int index = -1;
		for (int i = 0; i < windows.size(); i++)
			if (windows[i].window == window)
			{
				index = i;
				break;
			}
		if (index > windows.size() || index < 0)
			return;
		windows[index].scrollOffsetX = xoffset;
		windows[index].scrollOffsetY = yoffset;
	}
	static void SetCallbacks(unsigned int index)
	{
		glfwSetKeyCallback(windows[index].window, KeyCallback);
		glfwSetMouseButtonCallback(windows[index].window, ButtonCallback);
		glfwSetScrollCallback(windows[index].window, ScrollCallback);
	}
	static void RemoveCallbacks(unsigned int index)
	{
		glfwSetKeyCallback(windows[index].window, NULL);
		glfwSetMouseButtonCallback(windows[index].window, NULL);
		glfwSetScrollCallback(windows[index].window, NULL);
	}

	// public
	bool AddWindow(GLFWwindow* handle)
	{
		for (auto& it : windows)
			if (it.window == handle)
				return false;
		WindowState temp;
		temp.window = handle;
		windows.push_back(temp);
		SetCallbacks(windows.size()-1);
		return true;
	}
	bool RemoveWindow(GLFWwindow* handle)
	{
		for (int i = 0; i < windows.size(); i++)
			if (windows[i].window == handle)
			{
				RemoveCallbacks(i);
				windows.erase(windows.begin() + i);
				return true;
			}
		return false;
	}
	bool RemoveWindow(unsigned int index)
	{
		if (index > windows.size() || index < 0)
			return false;
		RemoveCallbacks(index);
		windows.erase(windows.begin() + index);
		return true;
	}
	void Poll()
	{
		for (auto& it : windows)
		{
			for (auto& key : it.keys)
			{
				key.second.press = false;
				key.second.release = false;
			}
			for (auto& key : it.buttons)
			{
				key.second.press = false;
				key.second.release = false;
			}
			it.scrollOffsetX = 0;
			it.scrollOffsetY = 0;
		}
	}
	int GetKey(GLFWwindow* handle, int key, int action)
	{
		int index = -1;
		for (int i = 0; i < windows.size(); i++)
			if (windows[i].window == handle)
			{
				index = i;
				break;
			}
		if (index > windows.size() || index < 0)
			return 0;
		if (action == Action::PRESS)
			return windows[index].keys[key].press;
		if (action == Action::RELEASE)
			return windows[index].keys[key].release;
		if (action == Action::HOLD)
			return windows[index].keys[key].hold;
		return 0;
	}
	int GetKey(unsigned int index, int key, int action)
	{
		if (index > windows.size() || index < 0)
			return 0;
		if (action == Action::PRESS)
			return windows[index].keys[key].press;
		if (action == Action::RELEASE)
			return windows[index].keys[key].release;
		if (action == Action::HOLD)
			return windows[index].keys[key].hold;
		return 0;
	}
	int GetMouseButton(GLFWwindow* handle, int button, int action)
	{
		int index = -1;
		for (int i = 0; i < windows.size(); i++)
			if (windows[i].window == handle)
			{
				index = i;
				break;
			}
		if (index > windows.size() || index < 0)
			return 0;
		if (action == Action::PRESS)
			return windows[index].buttons[button].press;
		if (action == Action::RELEASE)
			return windows[index].buttons[button].release;
		if (action == Action::HOLD)
			return windows[index].buttons[button].hold;
		return 0;
	}
	int GetMouseButton(unsigned int index, int button, int action)
	{
		if (index > windows.size() || index < 0)
			return 0;
		if (action == Action::PRESS)
			return windows[index].buttons[button].press;
		if (action == Action::RELEASE)
			return windows[index].buttons[button].release;
		if (action == Action::HOLD)
			return windows[index].buttons[button].hold;
		return 0;
	}
	glm::vec2 GetMousePosition(GLFWwindow* handle)
	{
		double x, y;
		int w, h;
		glfwGetFramebufferSize(handle, &w, &h);
		glfwGetCursorPos(handle, &x, &y);
		return { x, h - y };
	}
	glm::vec2 GetMousePosition(unsigned int index)
	{
		if (index > windows.size() || index < 0)
			return {};
		double x, y;
		int w, h;
		glfwGetFramebufferSize(windows[index].window, &w, &h);
		glfwGetCursorPos(windows[index].window, &x, &y);
		return { x, h - y };
	}
	void GetMousePosition(GLFWwindow* handle, double& x, double& y)
	{
		double temp;
		int w, h;
		glfwGetFramebufferSize(handle, &w, &h);
		glfwGetCursorPos(handle, &x, &temp);
		y = h - temp;
	}
	void GetMousePosition(unsigned int index, double& x, double& y)
	{
		if (index > windows.size() || index < 0)
			return;
		double temp;
		int w, h;
		glfwGetFramebufferSize(windows[index].window, &w, &h);
		glfwGetCursorPos(windows[index].window, &x, &temp);
		y = h - temp;
	}
	float GetScrollOffsetX(GLFWwindow* handle)
	{
		int index = -1;
		for (int i = 0; i < windows.size(); i++)
			if (windows[i].window == handle)
			{
				index = i;
				break;
			}
		if (index > windows.size() || index < 0)
			return 0;
		return windows[index].scrollOffsetX;
	}
	float GetScrollOffsetX(unsigned int index)
	{
		if (index > windows.size() || index < 0)
			return 0;
		return windows[index].scrollOffsetX;
	}
	float GetScrollOffsetY(GLFWwindow* handle)
	{
		int index = -1;
		for (int i = 0; i < windows.size(); i++)
			if (windows[i].window == handle)
			{
				index = i;
				break;
			}
		if (index > windows.size() || index < 0)
			return 0;
		return windows[index].scrollOffsetY;
	}
	float GetScrollOffsetY(unsigned int index)
	{
		if (index > windows.size() || index < 0)
			return 0;
		return windows[index].scrollOffsetY;
	}
}
