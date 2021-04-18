#include <iostream>

#include <app/application.h>

Application::Application(const std::string& title, const int width, const int height)
	: m_window(nullptr), m_input(nullptr)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (window == nullptr) {
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window");
	}
	m_window = window;
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		glfwTerminate();
		throw std::runtime_error("Failed to initialize GLAD");
	}

	glfwSetWindowUserPointer(m_window, reinterpret_cast<void*>(this));

	m_input = new Input();

	// none capture: lambda can implicit convert to function pointer

	auto framebufferSizeCallback = [](GLFWwindow* win, int width, int height)->void
	{
		glViewport(0, 0, width, height);
	};
	glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);

	auto scrollCallback = [](GLFWwindow* win, double xOffset, double yOffset) -> void
	{
		auto& app = *(static_cast<Application*>(glfwGetWindowUserPointer(win)));
		app.m_input->updateZoomOffset(static_cast<float>(yOffset));
	};
	glfwSetScrollCallback(m_window, scrollCallback);

	auto cursorPosCallback = [](GLFWwindow* win, double xPos, double yPos) -> void
	{
		auto& app = *(static_cast<Application*>(glfwGetWindowUserPointer(win)));
		app.m_input->updateCursorPos(static_cast<float>(xPos), static_cast<float>(yPos));
	};
	glfwSetCursorPosCallback(m_window, cursorPosCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}