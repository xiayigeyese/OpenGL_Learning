#include <iostream>
#include <array>
#include <vector>

#include <app/app.h>

#include "deferredShader.h"

using namespace std;
using namespace glm;

void initCube(VertexArray& vao, VertexBuffer<CubeVertex>& vbo)
{
	std::array<CubeVertex, 36> cubeVertices =
	{
		CubeVertex{{-0.5f, -0.5f, -0.5f},  {0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}}, // bottom-left
		CubeVertex{{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}}, // top-right
		CubeVertex{{ 0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 0.0f}}, // bottom-right         
		CubeVertex{{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}}, // top-right
		CubeVertex{{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}}, // bottom-left
		CubeVertex{{-0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 1.0f}}, // top-left
		CubeVertex{{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}}, // bottom-left
		CubeVertex{{ 0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 0.0f}}, // bottom-right
		CubeVertex{{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}}, // top-right
		CubeVertex{{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}}, // top-right
		CubeVertex{{-0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 1.0f}}, // top-left
		CubeVertex{{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}}, // bottom-left
		CubeVertex{{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}}, // top-right
		CubeVertex{{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}}, // top-left
		CubeVertex{{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}}, // bottom-left
		CubeVertex{{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}}, // bottom-left
		CubeVertex{{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}}, // bottom-right
		CubeVertex{{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}}, // top-right
		CubeVertex{{ 0.5f,  0.5f,  0.5f}, {1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}}, // top-left
		CubeVertex{{ 0.5f, -0.5f, -0.5f}, {1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}}, // bottom-right
		CubeVertex{{ 0.5f,  0.5f, -0.5f}, {1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}}, // top-right         
		CubeVertex{{ 0.5f, -0.5f, -0.5f}, {1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}}, // bottom-right
		CubeVertex{{ 0.5f,  0.5f,  0.5f}, {1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}}, // top-left
		CubeVertex{{ 0.5f, -0.5f,  0.5f}, {1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}}, // bottom-left     
		CubeVertex{{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}}, // top-right
		CubeVertex{{ 0.5f, -0.5f, -0.5f}, {0.0f, -1.0f,  0.0f}, {1.0f, 1.0f}}, // top-left
		CubeVertex{{ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}}, // bottom-left
		CubeVertex{{ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}}, // bottom-left
		CubeVertex{{-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}}, // bottom-right
		CubeVertex{{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}}, // top-right
		CubeVertex{{-0.5f,  0.5f, -0.5f}, {0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}}, // top-left
		CubeVertex{{ 0.5f,  0.5f , 0.5f}, {0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}}, // bottom-right
		CubeVertex{{ 0.5f,  0.5f, -0.5f}, {0.0f,  1.0f,  0.0f}, {1.0f, 1.0f}}, // top-right     
		CubeVertex{{ 0.5f,  0.5f,  0.5f}, {0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}}, // bottom-right
		CubeVertex{{-0.5f,  0.5f, -0.5f}, {0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}}, // top-left
		CubeVertex{{-0.5f,  0.5f,  0.5f}, {0.0f,  1.0f,  0.0f}, {0.0f, 0.0f}}  // bottom-left 
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

void testDeferredShader()
{
	// init app
	const int width = 800, height = 600;
	Application app("deferredShader", width, height);
	GLFWwindow* window = app.getWindow();
	Input* input = app.getInput();

	// set camera
	Camera camera = Camera::perspectiveCamera(
		vec3(0, 0, 3),
		vec3(0, 0, 0),
		vec3(0, 1, 0),
		45.0f,
		static_cast<float>(width) / height,
		0.1f,
		100.0f
	);
	CameraController cameraController(&camera, input);

	// set cube data
	VertexArray cubeVAO, quadVAO;
	VertexBuffer<CubeVertex> cubeVBO;
	VertexBuffer<QuadVertex> quadVBO;
	initCube(cubeVAO, cubeVBO);
	initQuad(quadVAO, quadVBO);

	// modelMatrix : cube position in world
	array<vec3, 10>  cubePositions = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	array<mat4, 10> objectModels = {};
	mat4 model;
	for(int i=0;i<10;i++)
	{
		model = mat4(1.0);
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * static_cast<float>(i);
		objectModels[i] = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
	}

	// set light
	unsigned int lightsNum = 4;
	// position
	std::array<glm::vec3, 4> lightPositions = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};
	// model matrix
	std::vector<glm::mat4> lightModelMatrix(lightsNum);
	for (size_t i = 0; i < lightsNum; i++)
	{		
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPositions[i]);
		lightModelMatrix[i] = glm::scale(model, glm::vec3(0.25f));
	}
	// color
	std::array<glm::vec3, 4> lightColors = {
		glm::vec3(5.0f, 5.0f, 5.0f),
		glm::vec3(10.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 10.0f),
		glm::vec3(0.0f, 10.0f, 0.0f)
	};
	// attenuation
	float constant = 1.0f;
	float linear = 0.7f;
	float quadratic = 1.8f;
	glm::vec3 lightAttenuation = vec3(constant, linear, quadratic);

	// load texture
	Texture2D containerDiffuseTex, containerSpecularTex;
	containerDiffuseTex.loadFromFile("resources/textures/container2.png", 4);
	containerSpecularTex.loadFromFile("resources/textures/container2_specular.png", 4);

	// load shader
	ShaderProgram gBufferShader({
		VertexShader::loadFromFile("tests/deferredShaderTest/shaders/g_buffer.vert").getHandler(),
		FragmentShader::loadFromFile("tests/deferredShaderTest/shaders/g_buffer.frag").getHandler()
	});
	ShaderProgram deferredLigthShader({
		VertexShader::loadFromFile("tests/deferredShaderTest/shaders/deferred_light.vert").getHandler(),
		FragmentShader::loadFromFile("tests/deferredShaderTest/shaders/deferred_light.frag").getHandler()
	});
	ShaderProgram cubeLightShader({
		VertexShader::loadFromFile("tests/deferredShaderTest/shaders/cube_light.vert").getHandler(),
		FragmentShader::loadFromFile("tests/deferredShaderTest/shaders/cube_light.frag").getHandler()
	});
	ShaderProgram finalShader({
		VertexShader::loadFromFile("tests/deferredShaderTest/shaders/final.vert").getHandler(),
		FragmentShader::loadFromFile("tests/deferredShaderTest/shaders/final.frag").getHandler()
	});

	
	// set shader uniform
	
	// g_buffer -> vs
	UniformVariable<glm::mat4> gBuffer_vs_model = gBufferShader.getUniformVariable<glm::mat4>("model");
	UniformVariable<glm::mat4> gBuffer_vs_view = gBufferShader.getUniformVariable<glm::mat4>("view");
	UniformVariable<glm::mat4> gBuffer_vs_projection = gBufferShader.getUniformVariable<glm::mat4>("projection");
	// g_buffer -> fs
	gBufferShader.setUniformValue("diffuseMap", 0);
	gBufferShader.setUniformValue("specularMap", 1);

	// deferred light -> fs
	deferredLigthShader.setUniformValue("gPosition", 0);
	deferredLigthShader.setUniformValue("gNormal", 1);
	deferredLigthShader.setUniformValue("gAlbedoSpec", 2);
	deferredLigthShader.setUniformValue("lightAttenuation", lightAttenuation);
	UniformVariable<glm::vec3> deferredLight_fs_viewPos = deferredLigthShader.getUniformVariable<glm::vec3>("viewPos");
	for(size_t i=0;i<lightsNum;i++)
	{
		deferredLigthShader.setUniformValue("lights[" + to_string(i) + "].position", lightPositions[i]);
		deferredLigthShader.setUniformValue("lights[" + to_string(i) + "].color", lightColors[i]);
	}

	// cube light -> vs
	UniformVariable<glm::mat4> cubeLight_vs_model = cubeLightShader.getUniformVariable<glm::mat4>("model");
	UniformVariable<glm::mat4> cubeLight_vs_view = cubeLightShader.getUniformVariable<glm::mat4>("view");
	UniformVariable<glm::mat4> cubeLight_vs_projection = cubeLightShader.getUniformVariable<glm::mat4>("projection");
	// cube ligth -> fs
	UniformVariable<glm::vec3> cubeLight_fs_lightColor = cubeLightShader.getUniformVariable<glm::vec3>("lightColor");

	// final -> fs
	finalShader.setUniformValue("deferredLightFBO", 0);
	
	// g_buffer fbo
	Framebuffer gBufferFBO;
	// attach depthBuffer
	Renderbuffer depthBuffer;
	depthBuffer.allocateStorage(GL_DEPTH_COMPONENT32F, width, height);
	gBufferFBO.attachRenderBuffer(GL_DEPTH_ATTACHMENT, depthBuffer);
	// attach colorBuffer: position + normal + albedoSpec
	std::array<GLenum, 3> attachments = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	Texture2D gPositionBuffer;
	gPositionBuffer.setTexFormat(1, GL_RGB16F, width, height);
	gPositionBuffer.setTexFilterParameter(GL_NEAREST, GL_NEAREST);
	gBufferFBO.attachTexture2D(attachments[0], gPositionBuffer);
	Texture2D gNormalBuffer;
	gNormalBuffer.setTexFormat(1, GL_RGB16F, width, height);
	gNormalBuffer.setTexFilterParameter(GL_NEAREST, GL_NEAREST);
	gBufferFBO.attachTexture2D(attachments[1], gNormalBuffer);
	Texture2D gAlbedoSpecBuffer;
	gAlbedoSpecBuffer.setTexFormat(1, GL_RGBA16F, width, height);
	gAlbedoSpecBuffer.setTexFilterParameter(GL_NEAREST, GL_NEAREST);
	gBufferFBO.attachTexture2D(attachments[2], gAlbedoSpecBuffer);

	gBufferFBO.setColorBuffersToDraw(3, attachments.data());
	if (!gBufferFBO.isComplete())
	{
		std::cout << "g_buffer is not complete!" << std::endl;
		return;
	}

	// deferred light fbo
	Framebuffer deferredLightFBO;
	// attach depthBuffer
	Renderbuffer deferredDepthBuffer;
	deferredDepthBuffer.allocateStorage(GL_DEPTH_COMPONENT32F, width, height);
	deferredLightFBO.attachRenderBuffer(GL_DEPTH_ATTACHMENT, deferredDepthBuffer);
	// attach colorBuffer
	Texture2D defferredColorBuffer;
	defferredColorBuffer.setTexFormat(1, GL_RGB16F, width, height);
	defferredColorBuffer.setTexFilterParameter(GL_NEAREST, GL_NEAREST);
	deferredLightFBO.attachTexture2D(GL_COLOR_ATTACHMENT0, defferredColorBuffer);
	if (!deferredLightFBO.isComplete())
	{
		std::cout << "deferred light buffer is not complete!" << std::endl;
		return;
	}

	// openGL config
	glEnable(GL_DEPTH_TEST);
	
	// render
	while(!glfwWindowShouldClose(window))
	{
		app.getKeyPressInput();
		cameraController.processKeyPressInput();

		// render geometry to g_buffer
		gBufferFBO.bind();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		gBufferShader.use();
		gBufferShader.setUniformValue(gBuffer_vs_view, camera.getViewMatrix());
		gBufferShader.setUniformValue(gBuffer_vs_projection, camera.getProjectionMatrix());
		glActiveTexture(0);
		containerDiffuseTex.bindTexUnit(0);
		glActiveTexture(1);
		containerSpecularTex.bindTexUnit(1);
		cubeVAO.bind();
		for(int i=0;i<10;i++)
		{
			gBufferShader.setUniformValue(gBuffer_vs_model, objectModels[i]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		cubeVAO.unbind();
		gBufferShader.unUse();
		gBufferFBO.unbind();

		// deferred light shader
		deferredLightFBO.bind();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		deferredLigthShader.use();
		deferredLigthShader.setUniformValue(deferredLight_fs_viewPos, camera.getPosition());
		glActiveTexture(0);
		gPositionBuffer.bindTexUnit(0);
		glActiveTexture(1);
		gNormalBuffer.bindTexUnit(1);
		glActiveTexture(2);
		gAlbedoSpecBuffer.bindTexUnit(2);
		quadVAO.bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		quadVAO.unbind();
		deferredLigthShader.unUse();

		// depthBuffer: g_buferFBO -> deferredLightFBO
		glBlitNamedFramebuffer(gBufferFBO.getHandler(), deferredLightFBO.getHandler(), 0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		
		// render light cube
		cubeLightShader.use();
		cubeLightShader.setUniformValue(cubeLight_vs_view, camera.getViewMatrix());
		cubeLightShader.setUniformValue(cubeLight_vs_projection, camera.getProjectionMatrix());
		cubeVAO.bind();
		for(size_t i=0;i<lightsNum;i++)
		{
			cubeLightShader.setUniformValue(cubeLight_vs_model, lightModelMatrix[i]);
			cubeLightShader.setUniformValue(cubeLight_fs_lightColor, lightColors[i]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		cubeVAO.unbind();
		cubeLightShader.unUse();

		deferredLightFBO.unbind();

		// render to screen
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		finalShader.use();
		glActiveTexture(0);
		defferredColorBuffer.bindTexUnit(0);
		quadVAO.bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		quadVAO.unbind();
		finalShader.unUse();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}