#include <app/app.h>
#include <opengl/opengl.h>

#include "test_pbr.h"
#include "pass/skyboxPass.h"

using namespace std;
using namespace glm;

void test_diffuse()
{
	// init app
	const int width = 1000, height = 800;
	Application app("pbr_diffuse", width, height);
	GLFWwindow* window = app.getWindow();
	Input* input = app.getInput();

	// set camera
	Camera camera = Camera::perspectiveCamera(
		vec3(0, 0, 3),
		vec3(0, 0, 1),
		vec3(0, 1, 0),
		45.0f,
		static_cast<float>(width) / height,
		0.1f,
		100.0f
	);
	CameraController cameraController(&camera, input);

	// init sphere
	Sphere sphere(200);
	Cube cube;

	// lights
	// ------
	glm::vec3 lightPositions[] = {
		glm::vec3(-10.0f,  10.0f, 10.0f),
		glm::vec3(10.0f,  10.0f, 10.0f),
		glm::vec3(-10.0f, -10.0f, 10.0f),
		glm::vec3(10.0f, -10.0f, 10.0f),
	};
	glm::vec3 lightColors[] = {
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f)
	};

	// load texture
	Texture2D environmentMap;
	environmentMap.loadFromFile("resources/textures/hdr/newport_loft.hdr", 1);
	environmentMap.setTexWrapParameter(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	environmentMap.setTexFilterParameter(GL_LINEAR, GL_LINEAR);

	// load shader
	ShaderProgram enMapConvertShader({
		VertexShader::loadFromFile("tests/pbr/shaders/envMap_to_envCubeMap.vert").getHandler(),
		FragmentShader::loadFromFile("tests/pbr/shaders/envMap_to_envCubeMap.frag").getHandler()
		});
	ShaderProgram irrMapConvertShader({
		VertexShader::loadFromFile("tests/pbr/shaders/envCubeMap_to_irrMap.vert").getHandler(),
		FragmentShader::loadFromFile("tests/pbr/shaders/envCubeMap_to_irrMap.frag").getHandler()
		});
	ShaderProgram pbrShader({
		VertexShader::loadFromFile("tests/pbr/shaders/ibl_diffuse.vert").getHandler(),
		FragmentShader::loadFromFile("tests/pbr/shaders/ibl_diffuse.frag").getHandler()
		});

	// init fbo

	// envCubeMapFBO
	Framebuffer envCubeMapFBO;
	const int envCubeMapWidth = 1024, envCubeMapHeight = 1024;
	Renderbuffer envCubeMapFBODepth;
	envCubeMapFBODepth.allocateStorage(GL_DEPTH_COMPONENT32F, envCubeMapWidth, envCubeMapHeight);
	envCubeMapFBO.attachRenderBuffer(GL_DEPTH_ATTACHMENT, envCubeMapFBODepth);
	std::shared_ptr<CubeMap> envCubeMapFBOColor = make_shared<CubeMap>();
	envCubeMapFBOColor->setTexWrapParameter(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	envCubeMapFBOColor->setTexFilterParameter(GL_LINEAR, GL_LINEAR);
	envCubeMapFBOColor->setTexFormat(1, GL_RGB16F, envCubeMapWidth, envCubeMapHeight);

	// irradianceMapFBO
	Framebuffer irradianceMapFBO;
	const int irradianceMapWith = 64, irradianceMapHeight = 64;
	Renderbuffer irradianceMapFBODepth;
	irradianceMapFBODepth.allocateStorage(GL_DEPTH_COMPONENT32F, irradianceMapWith, irradianceMapHeight);
	irradianceMapFBO.attachRenderBuffer(GL_DEPTH_ATTACHMENT, irradianceMapFBODepth);
	std::shared_ptr<CubeMap> irradianceMapFBOColor = make_shared<CubeMap>();
	irradianceMapFBOColor->setTexWrapParameter(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	irradianceMapFBOColor->setTexFilterParameter(GL_LINEAR, GL_LINEAR);
	irradianceMapFBOColor->setTexFormat(1, GL_RGB16F, irradianceMapWith, irradianceMapHeight);

	// add pass 
	SkyBoxPass skyBoxPass(envCubeMapFBOColor, true);
	// SkyBoxPass skyBoxPass(irradianceMapFBOColor, true);

	// set shader uniform

	// enMapConvertShader --> vs
	UniformVariable<glm::mat4> enMapConvertShader_vs_u_mvp = enMapConvertShader.getUniformVariable<glm::mat4>("u_mvp");
	// enMapConvertShader --> fs
	enMapConvertShader.setUniformValue("u_envMap", 0);

	// irrMapConvertShader --> vs
	UniformVariable<glm::mat4> irrMapConvertShader_vs_u_mvp = irrMapConvertShader.getUniformVariable<glm::mat4>("u_mvp");
	// irrMapConvertShader --> fs
	irrMapConvertShader.setUniformValue("u_envCubeMap", 0);

	// pbrShader --> vs
	UniformVariable<glm::mat4> pbrShader_vs_model = pbrShader.getUniformVariable<glm::mat4>("u_model");
	UniformVariable<glm::mat4> pbrShader_vs_mvp = pbrShader.getUniformVariable<glm::mat4>("u_mvp");
	// pbrShader --> fs
	for (int i = 0; i < 4; i++)
	{
		pbrShader.setUniformValue("light[" + to_string(i) + "].position", lightPositions[i]);
		pbrShader.setUniformValue("light[" + to_string(i) + "].color", lightColors[i]);
	}
	UniformVariable<vec3> shader_fs_viewPos = pbrShader.getUniformVariable<glm::vec3>("viewPos");
	pbrShader.setUniformValue("albedo", glm::vec3(0.5f, 0.0f, 0.0f));
	pbrShader.setUniformValue("ao", 1.0f);
	UniformVariable<float> pbrShader_fs_metallic = pbrShader.getUniformVariable<float>("metallic");
	UniformVariable<float> pbrShader_fs_roughness = pbrShader.getUniformVariable<float>("roughness");
	pbrShader.setUniformValue("irradianceMap", 0);

	// set sphere position
	int yNums = 7, xNums = 7;
	float spacing = 2.5;
	glm::mat4 model = glm::mat4(1.0f);
	vector<mat4> worldMatrix(yNums * xNums);
	vector<float> metallic(yNums);
	vector<float> roughness(xNums);
	for (int y = 0; y < yNums; y++)
	{
		metallic[y] = static_cast<float>(y) / static_cast<float>(yNums);
		for (int x = 0; x < xNums; x++)
		{
			roughness[x] = glm::clamp(static_cast<float>(x) / static_cast<float>(xNums), 0.05f, 1.0f);
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(
				static_cast<float>(x - (xNums / 2)) * spacing,
				static_cast<float>(y - (yNums / 2)) * spacing,
				0.0f
			));
			worldMatrix[x + y * xNums] = model;
		}
	}

	// openGL config
	glEnable(GL_DEPTH_TEST);

	// convert environmentMap to envCubeMap
	float near = 0.1f, far = 25.0f;
	float aspect = static_cast<float>(envCubeMapWidth) / envCubeMapHeight;
	glm::mat4 lightSpaceProjection = glm::perspective(glm::radians(90.0f), aspect, near, far);
	glm::vec3 eyePos = vec3(0, 0, 0);
	std::array<glm::mat4, 6> cubeFaceTransforms =
	{
		lightSpaceProjection * glm::lookAt(eyePos, eyePos + glm::vec3(1.0,  0.0,  0.0), glm::vec3(0.0, -1.0,  0.0)),
		lightSpaceProjection * glm::lookAt(eyePos, eyePos + glm::vec3(-1.0,  0.0,  0.0), glm::vec3(0.0, -1.0,  0.0)),
		lightSpaceProjection * glm::lookAt(eyePos, eyePos + glm::vec3(0.0,  1.0,  0.0), glm::vec3(0.0,  0.0,  1.0)),
		lightSpaceProjection * glm::lookAt(eyePos, eyePos + glm::vec3(0.0, -1.0,  0.0), glm::vec3(0.0,  0.0, -1.0)),
		lightSpaceProjection * glm::lookAt(eyePos, eyePos + glm::vec3(0.0,  0.0,  1.0), glm::vec3(0.0, -1.0,  0.0)),
		lightSpaceProjection * glm::lookAt(eyePos, eyePos + glm::vec3(0.0,  0.0, -1.0), glm::vec3(0.0, -1.0,  0.0)),
	};
	glViewport(0, 0, envCubeMapWidth, envCubeMapHeight);
	envCubeMapFBO.bind();
	enMapConvertShader.use();
	glActiveTexture(0);
	environmentMap.bindTexUnit(0);
	for (int i = 0; i < 6; i++)
	{
		envCubeMapFBO.attachCubeMapFace(GL_COLOR_ATTACHMENT0, *(envCubeMapFBOColor.get()), 0, i);
		if (!envCubeMapFBO.isComplete())
		{
			std::cout << " envCubeMapFBO is not complete! " << std::endl;
			return;
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		enMapConvertShader.setUniformValue(enMapConvertShader_vs_u_mvp, cubeFaceTransforms[i]);
		cube.draw();
	}
	enMapConvertShader.unUse();
	envCubeMapFBO.unbind();

	// convert envCubeMap to irradianceMap
	glViewport(0, 0, irradianceMapWith, irradianceMapHeight);
	irradianceMapFBO.bind();
	irrMapConvertShader.use();
	glActiveTexture(0);
	envCubeMapFBOColor->bindTexUnit(0);
	for (int i = 0; i < 6; i++)
	{
		irradianceMapFBO.attachCubeMapFace(GL_COLOR_ATTACHMENT0, *(irradianceMapFBOColor.get()), 0, i);
		if (!irradianceMapFBO.isComplete())
		{
			std::cout << "irradianceMapFBO is not complete! " << std::endl;
			return;
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		irrMapConvertShader.setUniformValue(irrMapConvertShader_vs_u_mvp, cubeFaceTransforms[i]);
		cube.draw();
	}
	irrMapConvertShader.unUse();
	irradianceMapFBO.unbind();

	// render
	glViewport(0, 0, width, height);
	while (!glfwWindowShouldClose(window))
	{
		app.getKeyPressInput();
		cameraController.processKeyPressInput();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		pbrShader.use();
		glActiveTexture(0);
		irradianceMapFBOColor->bindTexUnit(0);
		pbrShader.setUniformValue(shader_fs_viewPos, camera.getPosition());
		for (int y = 0; y < yNums; y++)
		{
			pbrShader.setUniformValue(pbrShader_fs_metallic, metallic[y]);
			for (int x = 0; x < xNums; x++)
			{
				pbrShader.setUniformValue(pbrShader_fs_roughness, roughness[x]);
				int index = x + y * xNums;
				pbrShader.setUniformValue(pbrShader_vs_model, worldMatrix[index]);
				glm::mat4 mvp = camera.getProjectionMatrix() * camera.getViewMatrix() * worldMatrix[index];
				pbrShader.setUniformValue(pbrShader_vs_mvp, mvp);
				sphere.draw();
			}
		}
		pbrShader.unUse();

		skyBoxPass.renderPass(camera.getViewMatrix(), camera.getProjectionMatrix());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}