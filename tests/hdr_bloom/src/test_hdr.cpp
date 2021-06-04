#include <iostream>
#include <string>

#include <array>

#include <app/app.h>

#include "test_hdr.h"

using namespace std;
using namespace glm;

void initCube(VertexArray& vao, VertexBuffer<CubeVertex>& vbo)
{
	std::array<CubeVertex, 36> cubeVertices =
	{
		CubeVertex{{-1.0f, -1.0f, -1.0f},  {0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}}, // bottom-left
		CubeVertex{{ 1.0f,  1.0f, -1.0f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}}, // top-right
		CubeVertex{{ 1.0f, -1.0f, -1.0f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 0.0f}}, // bottom-right         
		CubeVertex{{ 1.0f,  1.0f, -1.0f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}}, // top-right
		CubeVertex{{-1.0f, -1.0f, -1.0f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}}, // bottom-left
		CubeVertex{{-1.0f,  1.0f, -1.0f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 1.0f}}, // top-left
		CubeVertex{{-1.0f, -1.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}}, // bottom-left
		CubeVertex{{ 1.0f, -1.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 0.0f}}, // bottom-right
		CubeVertex{{ 1.0f,  1.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}}, // top-right
		CubeVertex{{ 1.0f,  1.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}}, // top-right
		CubeVertex{{-1.0f,  1.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 1.0f}}, // top-left
		CubeVertex{{-1.0f, -1.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}}, // bottom-left
		CubeVertex{{-1.0f,  1.0f,  1.0f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}}, // top-right
		CubeVertex{{-1.0f,  1.0f, -1.0f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}}, // top-left
		CubeVertex{{-1.0f, -1.0f, -1.0f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}}, // bottom-left
		CubeVertex{{-1.0f, -1.0f, -1.0f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}}, // bottom-left
		CubeVertex{{-1.0f, -1.0f,  1.0f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}}, // bottom-right
		CubeVertex{{-1.0f,  1.0f,  1.0f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}}, // top-right
		CubeVertex{{ 1.0f,  1.0f,  1.0f}, {1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}}, // top-left
		CubeVertex{{ 1.0f, -1.0f, -1.0f}, {1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}}, // bottom-right
		CubeVertex{{ 1.0f,  1.0f, -1.0f}, {1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}}, // top-right         
		CubeVertex{{ 1.0f, -1.0f, -1.0f}, {1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}}, // bottom-right
		CubeVertex{{ 1.0f,  1.0f,  1.0f}, {1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}}, // top-left
		CubeVertex{{ 1.0f, -1.0f,  1.0f}, {1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}}, // bottom-left     
		CubeVertex{{-1.0f, -1.0f, -1.0f}, {0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}}, // top-right
		CubeVertex{{ 1.0f, -1.0f, -1.0f}, {0.0f, -1.0f,  0.0f}, {1.0f, 1.0f}}, // top-left
		CubeVertex{{ 1.0f, -1.0f,  1.0f}, {0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}}, // bottom-left
		CubeVertex{{ 1.0f, -1.0f,  1.0f}, {0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}}, // bottom-left
		CubeVertex{{-1.0f, -1.0f,  1.0f}, {0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}}, // bottom-right
		CubeVertex{{-1.0f, -1.0f, -1.0f}, {0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}}, // top-right
		CubeVertex{{-1.0f,  1.0f, -1.0f}, {0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}}, // top-left
		CubeVertex{{ 1.0f,  1.0f , 1.0f}, {0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}}, // bottom-right
		CubeVertex{{ 1.0f,  1.0f, -1.0f}, {0.0f,  1.0f,  0.0f}, {1.0f, 1.0f}}, // top-right     
		CubeVertex{{ 1.0f,  1.0f,  1.0f}, {0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}}, // bottom-right
		CubeVertex{{-1.0f,  1.0f, -1.0f}, {0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}}, // top-left
		CubeVertex{{-1.0f,  1.0f,  1.0f}, {0.0f,  1.0f,  0.0f}, {0.0f, 0.0f}}  // bottom-left 
	};
	vbo.setData(cubeVertices.data(), cubeVertices.size());
	VertexAttrib positionAttrib = { 0, 3, GL_FLOAT, offsetof(CubeVertex, position) };
	VertexAttrib normalAttrib = { 1, 3, GL_FLOAT, offsetof(CubeVertex, normal) };
	VertexAttrib uvCoordsAttrib = { 2, 2, GL_FLOAT, offsetof(CubeVertex, uvCoords) };
	unsigned int bindingIndex = 0;
	vao.bindVertexBuffer(bindingIndex, vbo, 0);
	vao.bindVertexArrayAttrib(bindingIndex, positionAttrib, GL_FALSE);
	vao.bindVertexArrayAttrib(bindingIndex, normalAttrib, GL_FALSE);
	vao.bindVertexArrayAttrib(bindingIndex, uvCoordsAttrib, GL_FALSE);
}

void initQuad(VertexArray& vao, VertexBuffer<QuadVertex>& vbo)
{
	array<QuadVertex, 4> quadVertices =
	{
		QuadVertex{ {-1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
		QuadVertex{ { -1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
		QuadVertex{ { 1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
		QuadVertex{ { 1.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
	};
	vbo.setData(quadVertices.data(), quadVertices.size());
	VertexAttrib positionAttrib = { 0, 3, GL_FLOAT, offsetof(QuadVertex, position) };
	VertexAttrib uvCoordsAttrib = { 1,2,GL_FLOAT, offsetof(QuadVertex, uvCoords) };
	unsigned int bindingIndex = 0;
	vao.bindVertexBuffer(bindingIndex, vbo, 0);
	vao.bindVertexArrayAttrib(bindingIndex, positionAttrib, GL_FALSE);
	vao.bindVertexArrayAttrib(bindingIndex, uvCoordsAttrib, GL_FALSE);
}

void test_hdr()
{
	// init app
	const string title = "hdr";
	const int width = 800, height = 600;
	Application app(title, width, height);
	GLFWwindow* window = app.getWindow();
	Input* input = app.getInput();

	// set camera
	Camera camera = Camera::perspectiveCamera(
		vec3(0, 0, 1),
		vec3(0, 0, 2),
		vec3(0, 1, 0),
		45.0f,
		static_cast<float>(width) / height,
		0.1f,
		100.0f
	);
	CameraController cameraController(&camera, input);

	// set hdr fbo
	Framebuffer hdrFbo;
	// attach depth buffer and color buffer
	Renderbuffer depthBuffer;
	depthBuffer.allocateStorage(GL_DEPTH_COMPONENT16, width, height);
	hdrFbo.attachRenderBuffer(GL_DEPTH_ATTACHMENT, depthBuffer);
	Texture2D colorBuffer;
	colorBuffer.setTexFormat(1, GL_RGBA16F, width, height);
	colorBuffer.setTexFilterParameter(GL_LINEAR, GL_LINEAR);
	hdrFbo.attachTexture2D(GL_COLOR_ATTACHMENT0, colorBuffer);
	if (!hdrFbo.isComplete())
	{
		std::cout << "fbo is not complete!" << std::endl;
		return;
	}
	
	// set data
	VertexArray cubeVAO, quadVAO;
	VertexBuffer<CubeVertex> cubeVBO;
	VertexBuffer<QuadVertex> quadVBO;
	initCube(cubeVAO, cubeVBO);
	initQuad(quadVAO, quadVBO);

	// set light
	// positions
	std::vector<glm::vec3> lightPositions;
	lightPositions.push_back(glm::vec3(0.0f, 0.0f, 49.5f)); // back light
	lightPositions.push_back(glm::vec3(-1.4f, -1.9f, 9.0f));
	lightPositions.push_back(glm::vec3(0.0f, -1.8f, 4.0f));
	lightPositions.push_back(glm::vec3(0.8f, -1.7f, 6.0f));
	// colors
	std::vector<glm::vec3> lightColors;
	lightColors.push_back(glm::vec3(200.0f, 200.0f, 200.0f));
	lightColors.push_back(glm::vec3(0.1f, 0.0f, 0.0f));
	lightColors.push_back(glm::vec3(0.0f, 0.0f, 0.2f));
	lightColors.push_back(glm::vec3(0.0f, 0.1f, 0.0f));

	// load texture
	Texture2D diffuseMap;
	diffuseMap.loadFromFile("resources/textures/wood.png", 6);
	
	// load shader
	ShaderProgram hdrFboShader({
		VertexShader::loadFromFile("tests/hdr_bloom/shaders/hdr_fbo.vert").getHandler(),
		FragmentShader::loadFromFile("tests/hdr_bloom/shaders/hdr_fbo.frag").getHandler()
	});
	ShaderProgram hdrMappintShader({
		VertexShader::loadFromFile("tests/hdr_bloom/shaders/hdr_toneMapping.vert").getHandler(),
		FragmentShader::loadFromFile("tests/hdr_bloom/shaders/hdr_toneMapping.frag").getHandler()
	});

	// set shader uniform
	// fbo -> vs
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 25.0));
	model = glm::scale(model, glm::vec3(2.5f, 2.5f, 27.5f));
	hdrFboShader.setUniformValue("model", model);
	hdrFboShader.setUniformValue("reverseNormal", true);
	UniformVariable<glm::mat4> fbo_vs_view = hdrFboShader.getUniformVariable<glm::mat4>("view");
	UniformVariable<glm::mat4> fbo_vs_projection = hdrFboShader.getUniformVariable<glm::mat4>("projection");
	// fbo -> fs
	hdrFboShader.setUniformValue("diffuseMap", 0);
	for(int i=0;i<4;i++)
	{
		hdrFboShader.setUniformValue("lights[" + to_string(i) + "].position", lightPositions[i]);
		hdrFboShader.setUniformValue("lights[" + to_string(i) + "].color", lightColors[i]);
	}

	// mappint -> fs
	hdrMappintShader.setUniformValue("hdrFbo", 0);
	hdrMappintShader.setUniformValue("exposure", 1.0f);
	
	glEnable(GL_DEPTH_TEST);
	while(!glfwWindowShouldClose(window))
	{
		app.getKeyPressInput();
		cameraController.processKeyPressInput();

		hdrFbo.bind();
		glClearColor(0.1f, 0.1f, 0.1f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		hdrFboShader.use();
		hdrFboShader.setUniformValue(fbo_vs_view, camera.getViewMatrix());
		hdrFboShader.setUniformValue(fbo_vs_projection, camera.getProjectionMatrix());
		glActiveTexture(0);
		diffuseMap.bindTexUnit(0);
		cubeVAO.bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
		cubeVAO.unbind();
		hdrFboShader.unUse();
		hdrFbo.unbind();

		glClearColor(0.1f, 0.1f, 0.1f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		hdrMappintShader.use();
		glActiveTexture(0);
		colorBuffer.bindTexUnit(0);
		quadVAO.bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		quadVAO.unbind();
		hdrMappintShader.unUse();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}
