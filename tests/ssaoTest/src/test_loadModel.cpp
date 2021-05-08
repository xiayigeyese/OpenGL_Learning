#include <iostream>
#include <app/app.h>

#include "test_loadModel.h"

using namespace std;
using namespace glm;

void test_loadModel()
{
	// init app
	const int width = 1200, height = 800;
	Application app("load model", width, height);
	GLFWwindow* window = app.getWindow();
	Input* input = app.getInput();

	// init camera
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

	// load shader
	ShaderProgram shader({
		VertexShader::loadFromFile("tests/ssaoTest/shaders/load_model.vert").getHandler(),
		FragmentShader::loadFromFile("tests/ssaoTest/shaders/load_model.frag").getHandler()
	});

	// shader: set uniform
	UniformVariable<glm::mat4> s_vs_mvp = shader.getUniformVariable<glm::mat4>("mvp");
	shader.setUniformValue("diffuseMap", 0);

	// load model
	// string modelPath = "resources/objects/nanosuit/nanosuit.obj";
	string modelPath = "resources/objects/backpack/backpack.obj";
	Model model(modelPath, MATERIAL_SET::D, { 0 });
	
	// openGL config
	glEnable(GL_DEPTH_TEST);

	// render
	while(!glfwWindowShouldClose(window))
	{
		app.getKeyPressInput();
		cameraController.processKeyPressInput();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		shader.setUniformValue(s_vs_mvp, camera.getProjectionMatrix() * camera.getViewMatrix());
		model.draw(shader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}
