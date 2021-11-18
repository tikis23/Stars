#include "Window.h"
#include <sstream>
Window::Window(int width, int height, std::string title, int mode)
{
    widthOriginal = width;
    heightOriginal = height;
    this->title = title;
	const GLFWvidmode* monitorMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	if(mode == WINDOW_FULLSCREEN)
		handle = glfwCreateWindow(width, height, title.c_str(), glfwGetPrimaryMonitor(), NULL);
	else if (mode == WINDOW_BORDERLESS)
		handle = glfwCreateWindow(monitorMode->width, monitorMode->height, title.c_str(), glfwGetPrimaryMonitor(), NULL);
	else if (mode == WINDOW_WINDOWED)
		handle = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    glfwGetWindowPos(handle, &posXOriginal, &posYOriginal);
    glfwSetFramebufferSizeCallback(handle, Framebuffer_callback);
}

Window::~Window()
{
	glfwDestroyWindow(handle);
}

void Window::MakeCurrent()
{
	glfwMakeContextCurrent(handle);
}

void Window::Update()
{
    //ChangeTitle();
    glfwSwapBuffers(handle);
	glfwPollEvents();
}

void Window::ChangeMode(int mode)
{
    const GLFWvidmode* monitor = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if (mode == WINDOW_FULLSCREEN)
        glfwSetWindowMonitor(handle, glfwGetPrimaryMonitor(), 0, 0, monitor->width, monitor->height, GLFW_DONT_CARE);
    else if (mode == WINDOW_BORDERLESS)
        glfwSetWindowMonitor(handle, nullptr, 0, 0, monitor->width, monitor->height, GLFW_DONT_CARE);
    else if (mode == WINDOW_WINDOWED)
        glfwSetWindowMonitor(handle, nullptr, posXOriginal, posYOriginal, widthOriginal, heightOriginal, GLFW_DONT_CARE);
}

void Window::SetVSync(int mode)
{
	glfwSwapInterval(mode);
}

void Window::Framebuffer_callback(GLFWwindow* window, int width, int height)
{
    updated = true;
    glViewport(0, 0, width, height);
}

bool Window::updated = false;

void Window::ChangeTitle()
{
    double delta = glfwGetTime() - lastUpdate;
    fps++;
    if (delta >= 0.5)
    {
        double fpsTitle = double(fps) / delta;

        std::stringstream ss;
        ss << title << " [" << fpsTitle << " FPS]";

        glfwSetWindowTitle(handle, ss.str().c_str());

        fps = 0;
        lastUpdate = glfwGetTime();
    }
}