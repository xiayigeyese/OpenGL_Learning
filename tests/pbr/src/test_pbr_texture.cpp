#include <iostream>

#include <opengl/opengl.h>
#include <app/app.h>

#include "test_pbr.h"
#include "pass/skyboxPass.h"
#include "pass/iblPass.h"

using namespace std;
using namespace glm;

void test_pbr_texture()
{
	cout << std::log2(2048) << endl;
	
	// init app
	const int width = 1000, height = 800;
	Application app("pbr_texture", width, height);
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

	// init geometry
	Sphere sphere(200);
	vector<mat4> worldMatrix(5);
	glm::mat4 model = glm::mat4(1.0f);
	for(int i=0; i<5; i++)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-5.0 + i * 2, 0.0, 2.0));
		worldMatrix[i] = model;
	}

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

	// load hdr texture
	std::shared_ptr<Texture2D> envMap = std::make_shared<Texture2D>();
	envMap->loadFromFile("resources/textures/hdr/newport_loft.hdr", 1);
	envMap->setTexWrapParameter(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	envMap->setTexFilterParameter(GL_LINEAR, GL_LINEAR);

	// load PBR material texture
	// rusted iron
	Texture2D ironAlbedoMap, ironNormalMap, ironMetallicMap, ironRoughnessMap, ironAOMap;
	ironAlbedoMap.loadFromFile("resources/textures/pbr/rusted_iron/albedo.png", Texture2D::MAX_MIPMAP);
	ironNormalMap.loadFromFile("resources/textures/pbr/rusted_iron/normal.png", Texture2D::MAX_MIPMAP);
	ironMetallicMap.loadFromFile("resources/textures/pbr/rusted_iron/metallic.png", Texture2D::MAX_MIPMAP);
	ironRoughnessMap.loadFromFile("resources/textures/pbr/rusted_iron/roughness.png", Texture2D::MAX_MIPMAP);
	ironAOMap.loadFromFile("resources/textures/pbr/rusted_iron/ao.png", Texture2D::MAX_MIPMAP);
	// gold
	Texture2D goldAlbedoMap, goldNormalMap, goldMetallicMap, goldRoughnessMap, goldAOMap;
	goldAlbedoMap.loadFromFile("resources/textures/pbr/gold/albedo.png", Texture2D::MAX_MIPMAP);
	goldNormalMap.loadFromFile("resources/textures/pbr/gold/normal.png", Texture2D::MAX_MIPMAP);
	goldMetallicMap.loadFromFile("resources/textures/pbr/gold/metallic.png", Texture2D::MAX_MIPMAP);
	goldRoughnessMap.loadFromFile("resources/textures/pbr/gold/roughness.png", Texture2D::MAX_MIPMAP);
	goldAOMap.loadFromFile("resources/textures/pbr/gold/ao.png", Texture2D::MAX_MIPMAP);
	// grass
	Texture2D grassAlbedoMap, grassNormalMap, grassMetallicMap, grassRoughnessMap, grassAOMap;
	grassAlbedoMap.loadFromFile("resources/textures/pbr/grass/albedo.png", Texture2D::MAX_MIPMAP);
	grassNormalMap.loadFromFile("resources/textures/pbr/grass/normal.png", Texture2D::MAX_MIPMAP);
	grassMetallicMap.loadFromFile("resources/textures/pbr/grass/metallic.png", Texture2D::MAX_MIPMAP);
	grassRoughnessMap.loadFromFile("resources/textures/pbr/grass/roughness.png", Texture2D::MAX_MIPMAP);
	grassAOMap.loadFromFile("resources/textures/pbr/grass/ao.png", Texture2D::MAX_MIPMAP);
	// plastic
	Texture2D plasticAlbedoMap, plasticNormalMap, plasticMetallicMap, plasticRoughnessMap, plasticAOMap;
	plasticAlbedoMap.loadFromFile("resources/textures/pbr/plastic/albedo.png", Texture2D::MAX_MIPMAP);
	plasticNormalMap.loadFromFile("resources/textures/pbr/plastic/normal.png", Texture2D::MAX_MIPMAP);
	plasticMetallicMap.loadFromFile("resources/textures/pbr/plastic/metallic.png", Texture2D::MAX_MIPMAP);
	plasticRoughnessMap.loadFromFile("resources/textures/pbr/plastic/roughness.png", Texture2D::MAX_MIPMAP);
	plasticAOMap.loadFromFile("resources/textures/pbr/plastic/ao.png", Texture2D::MAX_MIPMAP);
	// wall
	Texture2D wallAlbedoMap, wallNormalMap, wallMetallicMap, wallRoughnessMap, wallAOMap;
	wallAlbedoMap.loadFromFile("resources/textures/pbr/wall/albedo.png", Texture2D::MAX_MIPMAP);
	wallNormalMap.loadFromFile("resources/textures/pbr/wall/normal.png", Texture2D::MAX_MIPMAP);
	wallMetallicMap.loadFromFile("resources/textures/pbr/wall/metallic.png", Texture2D::MAX_MIPMAP);
	wallRoughnessMap.loadFromFile("resources/textures/pbr/wall/roughness.png", Texture2D::MAX_MIPMAP);
	wallAOMap.loadFromFile("resources/textures/pbr/wall/ao.png", Texture2D::MAX_MIPMAP);
	
	// pbrShader
	ShaderProgram pbrShader({
		VertexShader::loadFromFile("tests/pbr/shaders/pbr_texture.vert").getHandler(),
		FragmentShader::loadFromFile("tests/pbr/shaders/pbr_texture.frag").getHandler()
		});

	// ibl pass
	IBLPass iblPass(envMap);
	// openGL config
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	iblPass.renderPass();
	std::shared_ptr<CubeMap> envCubeMap = iblPass.getEnvCubeMap();
	std::shared_ptr<CubeMap> irradianceMap = iblPass.getIrradianceMap();
	std::shared_ptr<CubeMap> prefilterMap = iblPass.getPrefilterMap();
	std::shared_ptr<Texture2D> brdfLut = iblPass.getBRDFLut();
	float maxPrefilterMapMipLevel = static_cast<float>(iblPass.getPrefilterMapMaxMipLevel());

	// skybox pass
	SkyBoxPass skyBoxPass(envCubeMap, true);

	// shader set uniform
	// pbrShader --> vs
	UniformVariable<glm::mat4> pbrS_vs_model = pbrShader.getUniformVariable<glm::mat4>("u_model");
	UniformVariable<glm::mat4> pbrS_vs_mvp = pbrShader.getUniformVariable<glm::mat4>("u_mvp");
	// pbrShader --> fs
	for (int i = 0; i < 4; i++)
	{
		pbrShader.setUniformValue("u_lights[" + to_string(i) + "].position", lightPositions[i]);
		pbrShader.setUniformValue("u_lights[" + to_string(i) + "].color", lightColors[i]);
	}
	UniformVariable<glm::vec3> pbrS_fs_viewPos = pbrShader.getUniformVariable<glm::vec3>("u_viewPos");
	pbrShader.setUniformValue("u_maxPrefilterMapMipLevel", maxPrefilterMapMipLevel);
	pbrShader.setUniformValue("u_irradianceMap", 0);
	pbrShader.setUniformValue("u_prefilterMap", 1);
	pbrShader.setUniformValue("u_brdfLUT", 2);
	pbrShader.setUniformValue("u_albedoMap", 3);
	pbrShader.setUniformValue("u_normalMap", 4);
	pbrShader.setUniformValue("u_metallicMap", 5);
	pbrShader.setUniformValue("u_roughnessMap", 6);
	pbrShader.setUniformValue("u_aoMap", 7);
	
	// render
	glViewport(0, 0, width, height);
	while(!glfwWindowShouldClose(window))
	{
		app.getKeyPressInput();
		cameraController.processKeyPressInput();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		pbrShader.use();
		glActiveTexture(0);
		irradianceMap->bindTexUnit(0);
		glActiveTexture(1);
		prefilterMap->bindTexUnit(1);
		glActiveTexture(2);
		brdfLut->bindTexUnit(2);
		pbrShader.setUniformValue(pbrS_fs_viewPos, camera.getPosition());
		glm::mat4 vp = camera.getProjectionMatrix() * camera.getViewMatrix();
		
		// rusted iron
		glActiveTexture(3);
		ironAlbedoMap.bindTexUnit(3);
		glActiveTexture(4);
		ironNormalMap.bindTexUnit(4);
		glActiveTexture(5);
		ironMetallicMap.bindTexUnit(5);
		glActiveTexture(6);
		ironRoughnessMap.bindTexUnit(6);
		glActiveTexture(7);
		ironAOMap.bindTexUnit(7);
		pbrShader.setUniformValue(pbrS_vs_model, worldMatrix[0]);
		pbrShader.setUniformValue(pbrS_vs_mvp, vp * worldMatrix[0]);
		sphere.draw();

		// gold
		glActiveTexture(3);
		goldAlbedoMap.bindTexUnit(3);
		glActiveTexture(4);
		goldNormalMap.bindTexUnit(4);
		glActiveTexture(5);
		goldMetallicMap.bindTexUnit(5);
		glActiveTexture(6);
		goldRoughnessMap.bindTexUnit(6);
		glActiveTexture(7);
		goldAOMap.bindTexUnit(7);
		pbrShader.setUniformValue(pbrS_vs_model, worldMatrix[1]);
		pbrShader.setUniformValue(pbrS_vs_mvp, vp * worldMatrix[1]);
		sphere.draw();

		// grass
		glActiveTexture(3);
		grassAlbedoMap.bindTexUnit(3);
		glActiveTexture(4);
		grassNormalMap.bindTexUnit(4);
		glActiveTexture(5);
		grassMetallicMap.bindTexUnit(5);
		glActiveTexture(6);
		grassRoughnessMap.bindTexUnit(6);
		glActiveTexture(7);
		grassAOMap.bindTexUnit(7);
		pbrShader.setUniformValue(pbrS_vs_model, worldMatrix[2]);
		pbrShader.setUniformValue(pbrS_vs_mvp, vp * worldMatrix[2]);
		sphere.draw();

		// grass
		glActiveTexture(3);
		plasticAlbedoMap.bindTexUnit(3);
		glActiveTexture(4);
		plasticNormalMap.bindTexUnit(4);
		glActiveTexture(5);
		plasticMetallicMap.bindTexUnit(5);
		glActiveTexture(6);
		plasticRoughnessMap.bindTexUnit(6);
		glActiveTexture(7);
		plasticAOMap.bindTexUnit(7);
		pbrShader.setUniformValue(pbrS_vs_model, worldMatrix[3]);
		pbrShader.setUniformValue(pbrS_vs_mvp, vp * worldMatrix[3]);
		sphere.draw();
		
		// wall
		glActiveTexture(3);
		wallAlbedoMap.bindTexUnit(3);
		glActiveTexture(4);
		wallNormalMap.bindTexUnit(4);
		glActiveTexture(5);
		wallMetallicMap.bindTexUnit(5);
		glActiveTexture(6);
		wallRoughnessMap.bindTexUnit(6);
		glActiveTexture(7);
		wallAOMap.bindTexUnit(7);
		pbrShader.setUniformValue(pbrS_vs_model, worldMatrix[4]);
		pbrShader.setUniformValue(pbrS_vs_mvp, vp* worldMatrix[4]);
		sphere.draw();

		pbrShader.unUse();

		skyBoxPass.renderPass(camera.getViewMatrix(), camera.getProjectionMatrix());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
}

