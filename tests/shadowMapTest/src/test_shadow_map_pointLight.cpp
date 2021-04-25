#include <string>
#include <array>

#include <app/app.h>

#include "test_shadow_map_pointLight.h"

void initCube(VertexArray& vao, VertexBuffer<CubeVertex>& vbo)
{
	std::array<CubeVertex, 36> cubeVertices =
	{
		// Back face
		CubeVertex{{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}}, // Bottom-left
		CubeVertex{{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}}, // top-right
		CubeVertex{{ 0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 0.0f}}, // bottom-right         
		CubeVertex{{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}}, // top-right
		CubeVertex{{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}}, // bottom-left
		CubeVertex{{-0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 1.0f}}, // top-left
		// Front face
		CubeVertex{{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}}, // bottom-left
		CubeVertex{{ 0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 0.0f}}, // bottom-right
		CubeVertex{{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}}, // top-right
		CubeVertex{{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}}, // top-right
		CubeVertex{{-0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 1.0f}}, // top-left
		CubeVertex{{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}}, // bottom-left
		// Left face
		CubeVertex{{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}}, // top-right
		CubeVertex{{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}}, // top-left
		CubeVertex{{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}}, // bottom-left
		CubeVertex{{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}}, // bottom-left
		CubeVertex{{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}}, // bottom-right
		CubeVertex{{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}}, // top-right
		// Right face
		CubeVertex{{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}}, // top-left
		CubeVertex{{ 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}}, // bottom-right
		CubeVertex{{ 0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}}, // top-right         
		CubeVertex{{ 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}}, // bottom-right
		CubeVertex{{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}}, // top-left
		CubeVertex{{ 0.5f, -0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}}, // bottom-left     
		// Bottom face
		CubeVertex{{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}}, // top-right
		CubeVertex{{ 0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f}}, // top-left
		CubeVertex{{ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}}, // bottom-left
		CubeVertex{{ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}}, // bottom-left
		CubeVertex{{-0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}}, // bottom-right
		CubeVertex{{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}}, // top-right
		// Top face
		CubeVertex{{-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}}, // top-left
		CubeVertex{{ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}}, // bottom-right
		CubeVertex{{ 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f}}, // top-right     
		CubeVertex{{ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}}, // bottom-right
		CubeVertex{{-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}}, // top-left
		CubeVertex{{-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 0.0f}}  // bottom-left    
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

void testShadowMapPointLight()
{
	// init app
	const std::string title = "shadow_map_pointLight";
	const int width = 800, height = 600;
	const Application app(title, width, height);
	GLFWwindow* window = app.getWindow();
	Input* input = app.getInput();

	// init camera
	Camera camera = Camera::perspectiveCamera(
		glm::vec3(0, 0, 0),
		glm::vec3(0, 0, -1),
		glm::vec3(0, 1, 0),
		45.0f,
		static_cast<float>(width) / height,
		0.1f,
		100.0f);
	CameraController cameraController(camera, *input);

	// set cube data
	VertexArray cubeVAO;
	VertexBuffer<CubeVertex> cubeVBO;
	initCube(cubeVAO, cubeVBO);

	// shadow map
	const int shadowMapWidth = 1024, shadowMapHeight = 1024;
	CubeMap cubeShadowMap = CubeMap::createCubeShadowMap(shadowMapWidth, shadowMapHeight);

	// fbo -> attach depth buffer
	Framebuffer fbo;
	fbo.attachCubeMap(GL_DEPTH_ATTACHMENT, cubeShadowMap);
	fbo.setColorBufferToDraw(GL_NONE);
	fbo.setColorBufferToRead(GL_NONE);
	if (!fbo.isComplete())
	{
		std::cout << "not complete" << std::endl;
		return;
	}

	// set light
	glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
	float near = 0.1f, far = 25.0f;
	float aspect = static_cast<float>(shadowMapWidth) / shadowMapHeight;
	glm::mat4 lightSpaceProjection = glm::perspective(glm::radians(90.0f), aspect, near, far);
	/*std::array<glm::mat4, 6> shadowTransforms =
	{
		lightSpaceProjection * glm::lookAt(lightPos, lightPos + glm::vec3( 1, 0, 0), glm::vec3(0, 1,  0)),
		lightSpaceProjection * glm::lookAt(lightPos, lightPos + glm::vec3(-1, 0, 0), glm::vec3(0, 1,  0)),
		lightSpaceProjection * glm::lookAt(lightPos, lightPos + glm::vec3( 0, 1, 0), glm::vec3(0, 0, -1)),
		lightSpaceProjection * glm::lookAt(lightPos, lightPos + glm::vec3( 0,-1, 0), glm::vec3(0, 0,  1)),
		lightSpaceProjection * glm::lookAt(lightPos, lightPos + glm::vec3( 0, 0, 1), glm::vec3(0, 1,  0)),
		lightSpaceProjection * glm::lookAt(lightPos, lightPos + glm::vec3( 0, 0,-1), glm::vec3(0, 1,  0)),
	};*/

	std::array<glm::mat4, 6> shadowTransforms =
	{
		lightSpaceProjection * glm::lookAt(lightPos, lightPos + glm::vec3(1.0,  0.0,  0.0), glm::vec3(0.0, -1.0,  0.0)),
		lightSpaceProjection * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0,  0.0,  0.0), glm::vec3(0.0, -1.0,  0.0)),
		lightSpaceProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0,  1.0,  0.0), glm::vec3(0.0,  0.0,  1.0)),
		lightSpaceProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0,  0.0), glm::vec3(0.0,  0.0, -1.0)),
		lightSpaceProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0,  0.0,  1.0), glm::vec3(0.0, -1.0,  0.0)),
		lightSpaceProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0,  0.0, -1.0), glm::vec3(0.0, -1.0,  0.0)),
	};

	// set shadow map shader
	ShaderProgram shadowShader({
		VertexShader::loadFromFile("tests/shadowMapTest/shaders/pointLight_shadow_depthMap.vert").getHandler(),
		GeometryShader::loadFromFile("tests/shadowMapTest/shaders/pointLight_shadow_depthMap.geom").getHandler(),
		FragmentShader::loadFromFile("tests/shadowMapTest/shaders/pointLight_shadow_depthMap.frag").getHandler()
	});
	UniformVariable<glm::mat4> shaderMap_vs_model = shadowShader.getUniformVariable<glm::mat4>("model");
	for(int i=0;i<6;i++)
	{
		shadowShader.setUniformValue("shadowTransforms[" + std::to_string(i) + "]", shadowTransforms[i]);
	}
	shadowShader.setUniformValue("farPlane", far);
	shadowShader.setUniformValue("lightPos", lightPos);

	// load texture
	Texture2D diffuseMap;
	diffuseMap.loadFromFile("resources/textures/wood.png", 3);
	
	// set world shader
	ShaderProgram worldShader({
		VertexShader::loadFromFile("tests/shadowMapTest/shaders/pointLight_shadow.vert").getHandler(),
		FragmentShader::loadFromFile("tests/shadowMapTest/shaders/pointLight_shadow.frag").getHandler()
	});
	worldShader.setUniformValue("diffuseMap", 0);
	worldShader.setUniformValue("shadowMap", 1);
	worldShader.setUniformValue("lightPos", lightPos);
	worldShader.setUniformValue("farPlane", far);
	UniformVariable<bool> ws_vs_reverseNormal = worldShader.getUniformVariable<bool>("reverseNormal");
	UniformVariable<glm::mat4> ws_vs_model = worldShader.getUniformVariable<glm::mat4>("model");
	UniformVariable<glm::mat4> ws_vs_view = worldShader.getUniformVariable<glm::mat4>("view");
	UniformVariable<glm::mat4> ws_vs_projection = worldShader.getUniformVariable<glm::mat4>("projection");
	UniformVariable<glm::vec3> ws_fs_viewPos = worldShader.getUniformVariable<glm::vec3>("viewPos");
	
	// object model matrix in world
	// room
	std::array<glm::mat4, 6> models = {};
	models[0] = glm::mat4(1.0f);
	models[0] = glm::scale(models[0], glm::vec3(10.0));
    // cube1
	models[1] = glm::mat4(1.0f);
	models[1] = glm::translate(models[1], glm::vec3(4.0f, -3.5f, 0.0));
	// cube2
	models[2] = glm::mat4(1.0f);
	models[2] = glm::translate(models[2], glm::vec3(2.0f, 3.0f, 1.0));
	models[2] = glm::scale(models[2], glm::vec3(1.5));
	// cube3
	models[3] = glm::mat4(1.0f);
	models[3] = glm::translate(models[3], glm::vec3(-3.0f, -1.0f, 0.0));
	// cube4
	models[4] = glm::mat4(1.0f);
	models[4] = glm::translate(models[4], glm::vec3(-1.5f, 1.0f, 1.5));
	// cube5
	models[5] = glm::mat4(1.0f);
	models[5] = glm::translate(models[5], glm::vec3(-1.5f, 2.0f, -3.0));
	models[5] = glm::rotate(models[5], 60.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	models[5] = glm::scale(models[5], glm::vec3(1.5));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	while(!glfwWindowShouldClose(window))
	{
		app.getKeyPressInput();
		cameraController.processKeyPressInput();

		glClearColor(0.1f, 0.1f, 0.1f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// render to shadowMap
		glViewport(0, 0, shadowMapWidth, shadowMapHeight);
		fbo.bind();
		glClear(GL_DEPTH_BUFFER_BIT);
		shadowShader.use();
		cubeVAO.bind();
		// room
		glDisable(GL_CULL_FACE);
		shadowShader.setUniformValue(shaderMap_vs_model, models[0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glEnable(GL_CULL_FACE);
		// cube
		glCullFace(GL_FRONT);
		for(int i                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               =1;i<6;i++)
		{
			shadowShader.setUniformValue(shaderMap_vs_model, models[i]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glCullFace(GL_BACK);
		cubeVAO.unBind();
		shadowShader.unUse();
		fbo.unBind();

		// render scene with shadowMap
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		worldShader.use();
		glActiveTexture(0);
		diffuseMap.bindTexUnit(0);
		glActiveTexture(1);
		cubeShadowMap.bindTexUnit(1);
		worldShader.setUniformValue(ws_vs_view, camera.getViewMatrix());
		worldShader.setUniformValue(ws_vs_projection, camera.getProjectionMatrix());
		worldShader.setUniformValue(ws_fs_viewPos, camera.getPosition());
		cubeVAO.bind();
		// room
		glDisable(GL_CULL_FACE);
		worldShader.setUniformValue(ws_vs_reverseNormal, true);
		worldShader.setUniformValue(ws_vs_model, models[0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		worldShader.setUniformValue(ws_vs_reverseNormal, false);
		glEnable(GL_CULL_FACE);
		// cube
		for(int i=1;i<6;i++)
		{
			worldShader.setUniformValue(ws_vs_model, models[i]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		cubeVAO.unBind();
		worldShader.unUse();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
}
