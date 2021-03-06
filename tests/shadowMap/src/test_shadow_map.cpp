#include <string>

#include <array>
#include <glm/gtc/matrix_transform.hpp>

#include <app/app.h>

#include "test_shadow_map.h"

Texture2D createShadowMap(const int width, const int height, const GLenum internalFormat)
{
	Texture2D shadow;
	shadow.setTexFormat(1, internalFormat, width, height);
	shadow.setTexFilterParameter(GL_NEAREST, GL_NEAREST);
	shadow.setTexWrapParameter(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
	std::array<float, 4> borderColor = {
		std::numeric_limits<float>::max(),
			std::numeric_limits<float>::max(),
			std::numeric_limits<float>::max(),
			1
	};
	shadow.setTexBorderColor(borderColor.data());
	return shadow;
}

void initBackVAO(VertexArray& backVAO, VertexBuffer<BackVertex>& vbo)
{
	std::array<BackVertex, 4> backVertices =
	{
		BackVertex{{-1.0f,  1.0f, 0.0f}, {0.0f, 1.0f}},
		BackVertex{{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
		BackVertex{{ 1.0f,  1.0f, 0.0f}, {1.0f, 1.0f}},
		BackVertex{{ 1.0f, -1.0f, 0.0f}, {1.0f, 0.0f}}
	};
	vbo.setData(backVertices.data(), backVertices.size());
	unsigned int bindingIndex = 0;
	backVAO.bindVertexBuffer(bindingIndex, vbo, 0);
	VertexAttrib positionAttrib = { 0, 3, GL_FLOAT, offsetof(BackVertex, position) };
	VertexAttrib uvCoordsAttrib = { 1, 2, GL_FLOAT, offsetof(BackVertex, uvCoords) };
	backVAO.bindVertexArrayAttrib(bindingIndex, positionAttrib, GL_FALSE);
	backVAO.bindVertexArrayAttrib(bindingIndex, uvCoordsAttrib, GL_FALSE);
}

void initCubeVAO(VertexArray& cubeVAO, VertexBuffer<CubeVertex>& vbo)
{
	std::array<CubeVertex, 36> cubeVertices =
	{
		// back face
		CubeVertex{{-1.0f, -1.0f, -1.0f},  {0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}}, // bottom-left
		CubeVertex{{ 1.0f,  1.0f, -1.0f},  {0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}}, // top-right
		CubeVertex{{ 1.0f, -1.0f, -1.0f},  {0.0f,  0.0f, -1.0f}, {1.0f, 0.0f}}, // bottom-right         
		CubeVertex{{ 1.0f,  1.0f, -1.0f},  {0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}}, // top-right
		CubeVertex{{-1.0f, -1.0f, -1.0f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}}, // bottom-left
		CubeVertex{{-1.0f,  1.0f, -1.0f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 1.0f}}, // top-left
		// front face
		CubeVertex{{-1.0f, -1.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}}, // bottom-left
		CubeVertex{{ 1.0f, -1.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 0.0f}}, // bottom-right
		CubeVertex{{ 1.0f,  1.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}}, // top-right
		CubeVertex{{ 1.0f,  1.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}}, // top-right
		CubeVertex{{-1.0f,  1.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 1.0f}}, // top-left
		CubeVertex{{-1.0f, -1.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}}, // bottom-left
		// left face
		CubeVertex{{-1.0f,  1.0f,  1.0f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}}, // top-right
		CubeVertex{{-1.0f,  1.0f, -1.0f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}}, // top-left
		CubeVertex{{-1.0f, -1.0f, -1.0f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}}, // bottom-left
		CubeVertex{{-1.0f, -1.0f, -1.0f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}}, // bottom-left
		CubeVertex{{-1.0f, -1.0f,  1.0f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}}, // bottom-right
		CubeVertex{{-1.0f,  1.0f,  1.0f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}}, // top-right
		// right face
		CubeVertex{{ 1.0f,  1.0f,  1.0f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}}, // top-left
		CubeVertex{{ 1.0f, -1.0f, -1.0f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}}, // bottom-right
		CubeVertex{{ 1.0f,  1.0f, -1.0f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}}, // top-right         
		CubeVertex{{ 1.0f, -1.0f, -1.0f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}}, // bottom-right
		CubeVertex{{ 1.0f,  1.0f,  1.0f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}}, // top-left
		CubeVertex{{ 1.0f, -1.0f,  1.0f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}}, // bottom-left     
		// bottom face
		CubeVertex{{-1.0f, -1.0f, -1.0f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}}, // top-right
		CubeVertex{{ 1.0f, -1.0f, -1.0f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f}}, // top-left
		CubeVertex{{ 1.0f, -1.0f,  1.0f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}}, // bottom-left
		CubeVertex{{ 1.0f, -1.0f,  1.0f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}}, // bottom-left
		CubeVertex{{-1.0f, -1.0f,  1.0f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}}, // bottom-right
		CubeVertex{{-1.0f, -1.0f, -1.0f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}}, // top-right
		// top face
		CubeVertex{{-1.0f,  1.0f, -1.0f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}}, // top-left
		CubeVertex{{ 1.0f,  1.0f , 1.0f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}}, // bottom-right
		CubeVertex{{ 1.0f,  1.0f, -1.0f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f}}, // top-right     
		CubeVertex{{ 1.0f,  1.0f,  1.0f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}}, // bottom-right
		CubeVertex{{-1.0f,  1.0f, -1.0f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}}, // top-left
		CubeVertex{{-1.0f,  1.0f,  1.0f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 0.0f}}  // bottom-left 
	};
	vbo.setData(cubeVertices.data(), cubeVertices.size());
	unsigned int bindingIndex = 0;
	VertexAttrib positionAttrib = { 0, 3, GL_FLOAT, offsetof(CubeVertex, position) };
	VertexAttrib normalAttrib = { 1, 3, GL_FLOAT, offsetof(CubeVertex, normal) };
	VertexAttrib uvCoodsAttrib = { 2, 2, GL_FLOAT, offsetof(CubeVertex, uvCoords) };
	cubeVAO.bindVertexBuffer(bindingIndex, vbo, 0);
	cubeVAO.bindVertexArrayAttrib(bindingIndex, positionAttrib, GL_FALSE);
	cubeVAO.bindVertexArrayAttrib(bindingIndex, normalAttrib, GL_FALSE);
	cubeVAO.bindVertexArrayAttrib(bindingIndex, uvCoodsAttrib, GL_FALSE);
}

void initPlaneVAO(VertexArray& planeVAO, VertexBuffer<PlaneVertex>& vbo)
{
	std::array<PlaneVertex, 6> planeVertices =
	{
		PlaneVertex{{ 25.0f, -0.5f,  25.0f}, {0.0f, 1.0f, 0.0f}, {25.0f,  0.0f}},
		PlaneVertex{{-25.0f, -0.5f,  25.0f}, {0.0f, 1.0f, 0.0f}, { 0.0f,  0.0f}},
		PlaneVertex{{-25.0f, -0.5f, -25.0f}, {0.0f, 1.0f, 0.0f}, { 0.0f, 25.0f}},
		PlaneVertex{{ 25.0f, -0.5f,  25.0f}, {0.0f, 1.0f, 0.0f}, {25.0f,  0.0f}},
		PlaneVertex{{-25.0f, -0.5f, -25.0f}, {0.0f, 1.0f, 0.0f}, { 0.0f, 25.0f}},
		PlaneVertex{{ 25.0f, -0.5f, -25.0f}, {0.0f, 1.0f, 0.0f}, {25.0f, 25.0f}}
	};
	vbo.setData(planeVertices.data(), planeVertices.size());
	unsigned int bindingIndex = 0;
	VertexAttrib positionAttrib = { 0, 3, GL_FLOAT, offsetof(CubeVertex, position) };
	VertexAttrib normalAttrib = { 1, 3, GL_FLOAT, offsetof(CubeVertex, normal) };
	VertexAttrib uvCoodsAttrib = { 2, 2, GL_FLOAT, offsetof(CubeVertex, uvCoords) };
	planeVAO.bindVertexBuffer(bindingIndex, vbo, 0);
	planeVAO.bindVertexArrayAttrib(bindingIndex, positionAttrib, GL_FALSE);
	planeVAO.bindVertexArrayAttrib(bindingIndex, normalAttrib, GL_FALSE);
	planeVAO.bindVertexArrayAttrib(bindingIndex, uvCoodsAttrib, GL_FALSE);
}

void testDepthMap()
{
	const std::string title = "OpenGL_learning";
	const int width = 800, height = 600;
	const Application app(title, width, height);
	GLFWwindow* window = app.getWindow();
	if (window == nullptr) return;

	// set data
	VertexArray backVAO, cubeVAO, planeVAO;
	VertexBuffer<BackVertex> backVBO;
	VertexBuffer<CubeVertex> cubeVBO;
	VertexBuffer<PlaneVertex> planeVBO;
	initBackVAO(backVAO, backVBO);
	initCubeVAO(cubeVAO, cubeVBO);
	initPlaneVAO(planeVAO, planeVBO);

	// set depthMap texture
	const int depthMapWidth = 256, depthMapHeight = 256;
	Texture2D depthMap;
	depthMap.setTexFormat(1, GL_DEPTH_COMPONENT32F, depthMapWidth, depthMapHeight);
	
	// set fbo and attach depthMap
	Framebuffer fbo;
	fbo.attachTexture2D(GL_DEPTH_ATTACHMENT, depthMap);
	// set color buffer not draw and read
	fbo.setColorBufferToDraw(GL_NONE);
	fbo.setColorBufferToRead(GL_NONE);

	// set light
	glm::vec3 lightPos(-2.0f, 20, -1.0f);
	float nearPlane = 1.0f, farPlane = 40;
	glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0, 1, 0));
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;


	// set depthMap shader
	ShaderProgram depthShader({
		VertexShader::loadFromFile("tests/shadowMap/shaders/depthMap.vert").getHandler(),
		FragmentShader::loadFromFile("tests/shadowMap/shaders/depthMap.frag").getHandler()
		});
	UniformVariable<glm::mat4> depth_vs_model = depthShader.getUniformVariable<glm::mat4>("model");
	UniformVariable<glm::mat4> depth_vs_lightSpaceMatrix = depthShader.getUniformVariable<glm::mat4>("lightSpaceMatrix");
	depthShader.setUniformValue(depth_vs_lightSpaceMatrix, lightSpaceMatrix);

	// set displayDepthMap shader
	ShaderProgram displayDepthShader({
		VertexShader::loadFromFile("tests/shadowMap/shaders/displayDepthMap.vert").getHandler(),
		FragmentShader::loadFromFile("tests/shadowMap/shaders/displayDepthMap.frag").getHandler()
		});
	displayDepthShader.use();
	displayDepthShader.setUniformValue("depthMap", 0);


	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// start render depthMap
		fbo.bind();
		glViewport(0, 0, depthMapWidth, depthMapHeight);
		glClear(GL_DEPTH_BUFFER_BIT);
		depthShader.use();

		// cube1
		glm::mat4 model1 = glm::mat4(1.0f);
		model1 = glm::translate(model1, glm::vec3(0.0f, 1.5f, 0.0));
		model1 = glm::scale(model1, glm::vec3(0.5f));
		depthShader.setUniformValue(depth_vs_model, model1);
		cubeVAO.bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
		cubeVAO.unbind();

		// cube2
		glm::mat4 model2 = glm::mat4(1.0);
		model2 = glm::translate(model2, glm::vec3(2.0f, 0.0f, 1.0));
		model2 = glm::scale(model2, glm::vec3(0.5f));
		depthShader.setUniformValue(depth_vs_model, model2);
		cubeVAO.bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
		cubeVAO.unbind();

		// cube3
		glm::mat4 model3 = glm::mat4(1.0f);
		model3 = glm::translate(model3, glm::vec3(-1.0f, 0.0f, 2.0));
		model3 = glm::rotate(model3, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		model3 = glm::scale(model3, glm::vec3(0.25));
		depthShader.setUniformValue(depth_vs_model, model3);
		cubeVAO.bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
		cubeVAO.unbind();

		// plane
		glm::mat4 model = glm::mat4(1.0f);
		depthShader.setUniformValue(depth_vs_model, model);
		planeVAO.bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		planeVAO.unbind();

		// end render depthMap
		fbo.unbind();

		// render 
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		displayDepthShader.use();
		backVAO.bind();
		glActiveTexture(GL_TEXTURE0);
		depthMap.bindTexUnit(0);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		backVAO.unbind();
		displayDepthShader.unUse();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}

void testShadowMap()
{
	// init app
	const std::string title = "OpenGL_learning";
	const int width = 800, height = 600;
	const Application app(title, width, height);
	GLFWwindow* window = app.getWindow();
	Input* input = app.getInput();

	// init camera
	Camera camera = Camera::perspectiveCamera(
		glm::vec3(0, 0, 3),
		glm::vec3(0, 0, 2),
		glm::vec3(0, 1, 0),
		45.0f,
		static_cast<float>(width) / height,
		0.1f, 
		100.0f);
	CameraController cameraController(&camera, input);

	// set data
	VertexArray cubeVAO, planeVAO;
	VertexBuffer<CubeVertex> cubeVBO;
	VertexBuffer<PlaneVertex> planeVBO;
	initCubeVAO(cubeVAO, cubeVBO);
	initPlaneVAO(planeVAO, planeVBO);

	// set light
	glm::vec3 lightPos(-2.0f, 8.0f, -1.0f);
	float near = 1.0f, far = 20.0f;
	glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0, 1, 0));
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near, far);
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	// shadow map
	int depthMapWidth = 1024, depthMapHeight = 1024;
	Texture2D depthMap = createShadowMap(depthMapWidth, depthMapHeight, GL_DEPTH_COMPONENT32F);

	// fbo -> attach depthMap to depthBuffer
    Framebuffer fbo;
	fbo.attachTexture2D(GL_DEPTH_ATTACHMENT, depthMap);
	fbo.setColorBufferToDraw(GL_NONE);
	fbo.setColorBufferToRead(GL_NONE);

	// depthShader
	ShaderProgram depthShader({
		VertexShader::loadFromFile("tests/shadowMapTest/shaders/depthMap.vert").getHandler(),
		FragmentShader::loadFromFile("tests/shadowMapTest/shaders/depthMap.frag").getHandler()
	});
	UniformVariable<glm::mat4> depthMap_vs_model = depthShader.getUniformVariable<glm::mat4>("model");
	// -> vs
	depthShader.setUniformValue("lightSpaceMatrix", lightSpaceMatrix);
	
	// load texture for object
	Texture2D diffuseMap;
	diffuseMap.loadFromFile("resources/textures/wood.png", 8);
	
	// load shader
	ShaderProgram shadowMapShader({
		VertexShader::loadFromFile("tests/shadowMapTest/shaders/shadowMap.vert").getHandler(),
		FragmentShader::loadFromFile("tests/shadowMapTest/shaders/shadowMap.frag").getHandler()
	});
    UniformVariable<glm::mat4> u_vs_model = shadowMapShader.getUniformVariable<glm::mat4>("model");
	UniformVariable<glm::mat4> u_vs_view = shadowMapShader.getUniformVariable<glm::mat4>("view");
	UniformVariable<glm::mat4> u_vs_projection = shadowMapShader.getUniformVariable<glm::mat4>("projection");
	UniformVariable<glm::vec3> u_fs_viewPos = shadowMapShader.getUniformVariable<glm::vec3>("viewPos");
	// -> vs
	shadowMapShader.setUniformValue("lightSpaceMatrix", lightSpaceMatrix);
	// -> fs
	shadowMapShader.setUniformValue("diffuseMap", 0);
	shadowMapShader.setUniformValue("depthMap", 1);
	shadowMapShader.setUniformValue("lightPos", lightPos);

	// world modelMatrix
	// cube1
	glm::mat4 model1 = glm::mat4(1.0f);
	model1 = glm::translate(model1, glm::vec3(0.0f, 1.5f, 0.0));
	model1 = glm::scale(model1, glm::vec3(0.5f));
	// cube2
	glm::mat4 model2 = glm::mat4(1.0);
	model2 = glm::translate(model2, glm::vec3(2.0f, 0.0f, 1.0));
	model2 = glm::scale(model2, glm::vec3(0.5f));
	// cube3
	glm::mat4 model3 = glm::mat4(1.0f);
	model3 = glm::translate(model3, glm::vec3(-1.0f, 0.0f, 2.0));
	model3 = glm::rotate(model3, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model3 = glm::scale(model3, glm::vec3(0.25));
	// plane
	glm::mat4 model = glm::mat4(1.0f);
	
	glEnable(GL_DEPTH_TEST);
	while(!glfwWindowShouldClose(window))
	{
		app.getKeyPressInput();
		cameraController.processKeyPressInput();
		
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// render to depthMap
		glCullFace(GL_FRONT);
		glViewport(0, 0, depthMapWidth, depthMapHeight);
		fbo.bind();
		glClear(GL_DEPTH_BUFFER_BIT);
		depthShader.use();
		// cube1, cube2, cube3
		cubeVAO.bind();
		depthShader.setUniformValue(depthMap_vs_model, model1);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		depthShader.setUniformValue(depthMap_vs_model, model2);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		depthShader.setUniformValue(depthMap_vs_model, model3);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		cubeVAO.unbind();
		// plane
		planeVAO.bind();
		depthShader.setUniformValue(depthMap_vs_model, model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		planeVAO.unbind();
		fbo.unbind();
		glCullFace(GL_BACK);

		// render with depthMap
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shadowMapShader.use();
		// -> vs
		shadowMapShader.setUniformValue(u_vs_view, camera.getViewMatrix());
		shadowMapShader.setUniformValue(u_vs_projection, camera.getProjectionMatrix());
		// -> fs
		shadowMapShader.setUniformValue(u_fs_viewPos, camera.getPosition());
		glActiveTexture(0);
		diffuseMap.bindTexUnit(0);
		glActiveTexture(1);
		depthMap.bindTexUnit(1);
		// cube1, cube2, cube3
		cubeVAO.bind();
		shadowMapShader.setUniformValue(u_vs_model, model1);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		shadowMapShader.setUniformValue(u_vs_model, model2);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		shadowMapShader.setUniformValue(u_vs_model, model3);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		cubeVAO.unbind();
		// plane
		planeVAO.bind();
		shadowMapShader.setUniformValue(u_vs_model, model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		planeVAO.unbind();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
}
