#include <app/app.h>
#include <opengl/opengl.h>

#include "test_lighting.h"
#include "pass/skyboxPass.h"

using namespace std;
using namespace glm;

void test_sphere()
{
	// init app
	const int width = 800, height = 600;
	Application app("sphere", width, height);
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

	// init data
	Sphere sphere(200);

	// load texture
	Texture2D diffuseMap;
	diffuseMap.loadFromFile("resources/textures/Kerbin.png", 12);
	diffuseMap.setTexWrapParameter(GL_REPEAT, GL_REPEAT);
	diffuseMap.setTexFilterParameter(GL_LINEAR, GL_LINEAR);

	// load shader
	ShaderProgram shader({
		VertexShader::loadFromFile("tests/pbr/shaders/sphere.vert").getHandler(),
		FragmentShader::loadFromFile("tests/pbr/shaders/sphere.frag").getHandler()
		});
	UniformVariable<glm::mat4> shader_vs_mvp = shader.getUniformVariable<glm::mat4>("u_mvp");
	shader.setUniformValue("diffuseMap", 0);
	
	// openGL config
	glEnable(GL_DEPTH_TEST);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render
	while(!glfwWindowShouldClose(window))
	{
		app.getKeyPressInput();
		cameraController.processKeyPressInput();

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		glActiveTexture(0);
		diffuseMap.bindTexUnit(0);
		shader.setUniformValue(shader_vs_mvp, camera.getProjectionMatrix() * camera.getViewMatrix());
		sphere.draw();
		shader.unUse();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	
}

void test_lighting()
{
	// init app
	const int width = 800, height = 600;
	Application app("pbr_lighting", width, height);
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


	// load shader
	ShaderProgram shader({
		VertexShader::loadFromFile("tests/pbr/shaders/pbr_light.vert").getHandler(),
		FragmentShader::loadFromFile("tests/pbr/shaders/pbr_light.frag").getHandler()
	});
	
	// set shader uniform
	// --> vs
	UniformVariable<glm::mat4> shader_vs_model = shader.getUniformVariable<glm::mat4>("u_model");
	UniformVariable<glm::mat4> shader_vs_mvp = shader.getUniformVariable<glm::mat4>("u_mvp");
	// --> fs
	for(int i=0;i<4;i++)
	{
		shader.setUniformValue("light[" + to_string(i) + "].position", lightPositions[i]);
		shader.setUniformValue("light[" + to_string(i) + "].color", lightColors[i]);
	}
	UniformVariable<vec3> shader_fs_viewPos = shader.getUniformVariable<glm::vec3>("viewPos");
	shader.setUniformValue("albedo", glm::vec3(0.5f, 0.0f, 0.0f));
	shader.setUniformValue("ao", 1.0f);
	UniformVariable<float> shader_fs_metallic = shader.getUniformVariable<float>("metallic");
	UniformVariable<float> shader_fs_roughness = shader.getUniformVariable<float>("roughness");

	
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

	// render
	while (!glfwWindowShouldClose(window))
	{
		app.getKeyPressInput();
		cameraController.processKeyPressInput();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		shader.setUniformValue(shader_fs_viewPos, camera.getPosition());
		for(int y=0;y<yNums;y++)
		{
			shader.setUniformValue(shader_fs_metallic, metallic[y]);
			for(int x=0; x < xNums;x++)
			{
				shader.setUniformValue(shader_fs_roughness, roughness[x]);
				int index = x + y * xNums;
				shader.setUniformValue(shader_vs_model, worldMatrix[index]);
				glm::mat4 mvp = camera.getProjectionMatrix() * camera.getViewMatrix() * worldMatrix[index];
				shader.setUniformValue(shader_vs_mvp, mvp);
				sphere.draw();
			}
		}
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}



