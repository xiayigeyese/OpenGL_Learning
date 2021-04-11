#pragma once

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <opengl/opengl.h>
#include "init.h"

using namespace std;
using namespace glm;

struct Vertex {
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 uvPos;
};

inline void testTexture()
{
	const int width = 1000, height = 800;

	//初始化openGL窗口和配置
	GLFWwindow* window = initWindow(width, height);

	if (window == nullptr)
	{
		return;
	}

	//创建并绑定ImGui
	initImGui(window);

	bool show_window = true;

	// 设置shader
	ShaderProgram shaderProgram({
		VertexShader::loadFromFile("tests/test1/shaders/test1.vert").getHandler(),
		FragmentShader::loadFromFile("tests/test1/shaders/test1.frag").getHandler()
		});
	UniformVariable<glm::mat4> u_vs_model = shaderProgram.getUniformVariable<glm::mat4>("model");


	//颜色数据
	ImVec4 topRightColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	ImVec4 buttonRightColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	ImVec4 buttonLeftColor = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
	ImVec4 topLeftColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);

	// 纹理数据
	Texture2D texture;
	texture.loadFromFile("resources/textures/container.png", GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);

	shaderProgram.use();
	shaderProgram.setUniformValue<int>("texture0", 0);

	std::vector<Vertex> vertices(8);
	vertices[0] = Vertex{ glm::vec3(0.5f,  0.5f, 0.0f), glm::vec3(), glm::vec2(1.0f, 1.0f) };
	vertices[1] = Vertex{ glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(), glm::vec2(1.0f, 0.0f) };
	vertices[2] = Vertex{ glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(),glm::vec2(0.0f, 0.0f) };
	vertices[3] = Vertex{ glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(), glm::vec2(0.0f, 1.0f) };

	unsigned int indices[] = {
			0, 1, 3,
			1, 2, 3
	};

	VertexArray vao;
	VertexBuffer<Vertex> vbo;
	Buffer ebo;

	unsigned int bindingIndex = 0;
	VertexAttrib positionAttrib = { 0, 3, GL_FLOAT, offsetof(Vertex, position) };
	VertexAttrib colorAttrib = { 1, 3, GL_FLOAT, offsetof(Vertex, color) };
	VertexAttrib uvPosAttrib = { 2, 2, GL_FLOAT, offsetof(Vertex, uvPos) };
	vao.bindVertexBuffer(bindingIndex, vbo, 0);
	vao.bindVertexArrayAttrib(bindingIndex, positionAttrib, GL_FALSE);
	vao.bindVertexArrayAttrib(bindingIndex, colorAttrib, GL_FALSE);
	vao.bindVertexArrayAttrib(bindingIndex, uvPosAttrib, GL_FALSE);

	ebo.setData(indices, sizeof(indices), GL_STATIC_DRAW);
	vao.bindElementBuffer(ebo);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		// imGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Edit color", &show_window, ImGuiWindowFlags_MenuBar);
		ImGui::ColorEdit3("topRight color", reinterpret_cast<float*>(&topRightColor));
		ImGui::ColorEdit3("buttonRight color", reinterpret_cast<float*>(&buttonRightColor));
		ImGui::ColorEdit3("buttonLeft color", reinterpret_cast<float*>(&buttonLeftColor));
		ImGui::ColorEdit3("topLeft color", reinterpret_cast<float*>(&topLeftColor));
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
			1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		// 更新颜色
		vertices[0].color = glm::vec3(topRightColor.x, topRightColor.y, topRightColor.z);
		vertices[1].color = glm::vec3(buttonRightColor.x, buttonRightColor.y, buttonRightColor.z);
		vertices[2].color = glm::vec3(buttonLeftColor.x, buttonLeftColor.y, buttonLeftColor.z);
		vertices[3].color = glm::vec3(topLeftColor.x, topLeftColor.y, topLeftColor.z);

		vbo.setData(vertices.data(), vertices.size(), GL_DYNAMIC_DRAW);
		shaderProgram.setUniformValue(u_vs_model, glm::mat4(1));

		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// bind Texture
		glActiveTexture(GL_TEXTURE0);
		texture.bindTexUnit(0);

		shaderProgram.use();
		vao.bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	vao.destroy();
	vbo.destroy();
	ebo.destroy();
	shaderProgram.destroy();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}