#include <iostream>

#include <app/app.h>

#include "test_bloom.h"

using namespace std;
using namespace glm;

void testBloom()
{
	// init app
	const int width = 800, height = 600;
	Application app("bloom", width, height);
	GLFWwindow* window = app.getWindow();
	Input* input = app.getInput();

	// set camera
	Camera camera = Camera::perspectiveCamera(
		vec3(0, 0, 5),
		vec3(0, 0, 4),
		vec3(0, 1, 0),
		45.0f,
		static_cast<float>(width) / height,
		0.1f,
		100.0f
	);
	CameraController cameraController(&camera, input);

	// set light
	// positions
	std::vector<glm::vec3> lightPositions;
	lightPositions.push_back(glm::vec3(0.0f, 0.5f, 1.5f));
	lightPositions.push_back(glm::vec3(-4.0f, 0.5f, -3.0f));
	lightPositions.push_back(glm::vec3(3.0f, 0.5f, 1.0f));
	lightPositions.push_back(glm::vec3(-.8f, 2.4f, -1.0f));
	// colors
	std::vector<glm::vec3> lightColors;
	lightColors.push_back(glm::vec3(5.0f, 5.0f, 5.0f));
	lightColors.push_back(glm::vec3(10.0f, 0.0f, 0.0f));
	lightColors.push_back(glm::vec3(0.0f, 0.0f, 15.0f));
	lightColors.push_back(glm::vec3(0.0f, 5.0f, 0.0f));

	// set object 
	VertexArray cubeVAO, quadVAO;
	VertexBuffer<CubeVertex> cubeVBO;
	VertexBuffer<QuadVertex> quadVBO;
	initCube(cubeVAO, cubeVBO);
	initQuad(quadVAO, quadVBO);

	// object world position -> modelMatrix
	// object
	std::array<mat4, 7> objectModels{};
	// large cube as plane
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0));
	objectModels[0] = glm::scale(model, glm::vec3(12.5f, 0.5f, 12.5f));
	// other cube
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
	objectModels[1] = glm::scale(model, glm::vec3(0.5f));
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
	objectModels[2] = glm::scale(model, glm::vec3(0.5f));
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 2.0));
	objectModels[3] = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 2.7f, 4.0));
	model = glm::rotate(model, glm::radians(23.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	objectModels[4] = glm::scale(model, glm::vec3(1.25));
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -3.0));
	objectModels[5] = glm::rotate(model, glm::radians(124.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0));
	objectModels[6] = glm::scale(model, glm::vec3(0.5f));
	// light cube
	std::array<mat4, 4> lightModel{};
	for(int i=0;i<4;i++)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(lightPositions[i]));
		lightModel[i] = glm::scale(model, glm::vec3(0.25f));
	}

	// load texture
	Texture2D woodTex, contianerTex;
	woodTex.loadFromFile("resources/textures/wood.png", 2);
	contianerTex.loadFromFile("resources/textures/container2.png", 3);

	// load shader
	ShaderProgram hdrFBOShader({
		VertexShader::loadFromFile("tests/hdr_bloomTest/shaders/bloom_hdrFBO.vert").getHandler(),
		FragmentShader::loadFromFile("tests/hdr_bloomTest/shaders/bloom_hdrFBO.frag").getHandler(),
	});
	ShaderProgram cubeLightShader({
		VertexShader::loadFromFile("tests/hdr_bloomTest/shaders/cube_light.vert").getHandler(),
		FragmentShader::loadFromFile("tests/hdr_bloomTest/shaders/cube_light.frag").getHandler(),
	});
	ShaderProgram blurHShader({
		VertexShader::loadFromFile("tests/hdr_bloomTest/shaders/bloom_blur.vert").getHandler(),
		FragmentShader::loadFromFile("tests/hdr_bloomTest/shaders/bloom_blur_GS_horizontal.frag").getHandler(),
	});
	ShaderProgram blurVShader({
		VertexShader::loadFromFile("tests/hdr_bloomTest/shaders/bloom_blur.vert").getHandler(),
		FragmentShader::loadFromFile("tests/hdr_bloomTest/shaders/bloom_blur_GS_vertical.frag").getHandler(),
	});
	ShaderProgram finalShader({
		VertexShader::loadFromFile("tests/hdr_bloomTest/shaders/bloom_final.vert").getHandler(),
		FragmentShader::loadFromFile("tests/hdr_bloomTest/shaders/bloom_final.frag").getHandler(),
	});

	// set shader uniform
	// hdrFBO -> vs
	UniformVariable<glm::mat4> hdrFBO_vs_model = hdrFBOShader.getUniformVariable<glm::mat4>("model");
	UniformVariable<glm::mat4> hdrFBO_vs_view = hdrFBOShader.getUniformVariable<glm::mat4>("view");
	UniformVariable<glm::mat4> hdrFBO_vs_projection = hdrFBOShader.getUniformVariable<glm::mat4>("projection");
	// hdrFBO ->fs
	hdrFBOShader.setUniformValue("diffuseMap", 0);
	for(int i=0;i<4;i++)
	{
		hdrFBOShader.setUniformValue("lights[" + to_string(i) + "].position", lightPositions[i]);
		hdrFBOShader.setUniformValue("lights[" + to_string(i) + "].color", lightColors[i]);
	}

	// cubeLight -> vs
	UniformVariable<glm::mat4> cubeLight_vs_model = cubeLightShader.getUniformVariable<glm::mat4>("model");
	UniformVariable<glm::mat4> cubeLight_vs_view = cubeLightShader.getUniformVariable<glm::mat4>("view");
	UniformVariable<glm::mat4> cubeLight_vs_projection = cubeLightShader.getUniformVariable<glm::mat4>("projection");
	// cubeLight -> fs
	UniformVariable<glm::vec3> cubeLight_fs_lightColor = cubeLightShader.getUniformVariable<glm::vec3>("lightColor");

	// blurH -> fs
	blurHShader.setUniformValue("fbo", 0);
	// blurV -> fs
	blurVShader.setUniformValue("fbo", 0);

	// final
	finalShader.setUniformValue("hdrFBO", 0);
	finalShader.setUniformValue("blurFBO", 1);
	UniformVariable<float> final_fs_exposure = finalShader.getUniformVariable<float>("exposure");

	// set fbo
	// hdrFBO
	Framebuffer hdrFBO;
	// attach depth buffer
	Renderbuffer depthBuffer;
	depthBuffer.allocateStorage(GL_DEPTH_COMPONENT16, width, height);
	hdrFBO.attachRenderBuffer(GL_DEPTH_ATTACHMENT, depthBuffer);
	// attach color buffer: hdrColorBuffer + prevBlurColorBuffer
	std::array<Texture2D, 2> colorBuffers{};
	std::array<GLenum, 2> attachments{};
	for(int i=0;i<2;i++)
	{
		colorBuffers[i].setTexFormat(1, GL_RGBA16F, width, height);
		colorBuffers[i].setTexFilterParameter(GL_LINEAR, GL_LINEAR);
		colorBuffers[i].setTexWrapParameter(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
		hdrFBO.attachTexture2D(GL_COLOR_ATTACHMENT0 + i, colorBuffers[i]);
		attachments[i] = GL_COLOR_ATTACHMENT0 + i;
	}
	hdrFBO.setColorBuffersToDraw(2, attachments.data());
	if (!hdrFBO.isComplete())
	{
		std::cout << "hdr fbo is not complete!" << std::endl;
		return;
	}
	
	// blurFBO: horizontal + vertical
	std::array<Framebuffer, 2> blurFBOs{};
	std::array<Texture2D, 2> blurFBOColorBuffers{};
	for(int i=0;i<2;i++)
	{
		blurFBOColorBuffers[i].setTexFormat(1, GL_RGBA16F, width, height);
		blurFBOColorBuffers[i].setTexFilterParameter(GL_LINEAR, GL_LINEAR);
		blurFBOColorBuffers[i].setTexWrapParameter(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
		blurFBOs[i].attachTexture2D(GL_COLOR_ATTACHMENT0, blurFBOColorBuffers[i]);
		if (!blurFBOs[i].isComplete())
		{
			std::cout << "The " << i << "th blur fbo is not complete!" << std::endl;
			return;
		}
	}

	// openGL config
	glEnable(GL_DEPTH_TEST);
	
	// render
	while(!glfwWindowShouldClose(window))
	{
		app.getKeyPressInput();
		cameraController.processKeyPressInput();

		// render to hdrFBO
		hdrFBO.bind();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// object 
		hdrFBOShader.use();
		hdrFBOShader.setUniformValue(hdrFBO_vs_view, camera.getViewMatrix());
		hdrFBOShader.setUniformValue(hdrFBO_vs_projection, camera.getProjectionMatrix());
		cubeVAO.bind();
		// big cube
		glActiveTexture(0);
		woodTex.bindTexUnit(0);
		hdrFBOShader.setUniformValue(hdrFBO_vs_model, objectModels[0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// other cube
		glActiveTexture(0);
		contianerTex.bindTexUnit(0);
		for(int i=1;i<7;i++)
		{
			hdrFBOShader.setUniformValue(hdrFBO_vs_model, objectModels[i]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		cubeVAO.unbind();
		hdrFBOShader.unUse();
		// light
		cubeLightShader.use();
		cubeLightShader.setUniformValue(cubeLight_vs_view, camera.getViewMatrix());
		cubeLightShader.setUniformValue(cubeLight_vs_projection, camera.getProjectionMatrix());
		cubeVAO.bind();
		for(int i=0;i<4;i++)
		{
			cubeLightShader.setUniformValue(cubeLight_vs_model, lightModel[i]);
			cubeLightShader.setUniformValue(cubeLight_fs_lightColor, lightColors[i]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		cubeLightShader.unUse();
		hdrFBO.unbind();

		// blur -> two pass: horizontal + vertical
		// horizontal
		blurFBOs[0].bind();
		glClear(GL_COLOR_BUFFER_BIT);
		blurHShader.use();
		// use hdrFBO 's prevBlurColorBuffer
		glActiveTexture(0);
		colorBuffers[1].bindTexUnit(0);
		quadVAO.bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		quadVAO.unbind();
		blurHShader.unUse();
		blurFBOs[0].unbind();
		// vertical
		blurFBOs[1].bind();
		glClear(GL_COLOR_BUFFER_BIT);
		blurVShader.use();
		glActiveTexture(0);
		blurFBOColorBuffers[0].bindTexUnit(0);
		quadVAO.bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		quadVAO.unbind();
		blurFBOs[1].unbind();
		// multi blur
		for(int i=1; i < 2;i++)
		{
			// horizontal
			blurFBOs[0].bind();
			glClear(GL_COLOR_BUFFER_BIT);
			blurHShader.use();
			glActiveTexture(0);
			blurFBOColorBuffers[1].bindTexUnit(0);
			quadVAO.bind();
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			quadVAO.unbind();
			blurHShader.unUse();
			blurFBOs[0].unbind();
			// vertical
			blurFBOs[1].bind();
			glClear(GL_COLOR_BUFFER_BIT);
			blurVShader.use();
			glActiveTexture(0);
			blurFBOColorBuffers[0].bindTexUnit(0);
			quadVAO.bind();
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			quadVAO.unbind();
			blurFBOs[1].unbind();
		}

		// final: tone mapping + concat + gamma
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		finalShader.use();
		glActiveTexture(0);
		colorBuffers[0].bindTexUnit(0);
		glActiveTexture(1);
		blurFBOColorBuffers[1].bindTexUnit(1);
		finalShader.setUniformValue(final_fs_exposure, 1.0f);
		quadVAO.bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		quadVAO.unbind();
		finalShader.unUse();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}
