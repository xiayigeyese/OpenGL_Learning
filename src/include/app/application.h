#pragma once

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "input.h"

class Application
{
public:
	Application(const std::string& title, const int width, const int height);

	~Application()
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

private:
	GLFWwindow* m_window;
	Input* m_input;
};