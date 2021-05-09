#include <iostream>
#include <array>
#include <random>

#include <app/app.h>

#include "test_ssao.h"

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
	VertexAttrib texCoordsAttrib = { 2, 2, GL_FLOAT, offsetof(CubeVertex, texCoords) };
	unsigned int bindingIndex = 0;
	vao.bindVertexBuffer(bindingIndex, vbo, 0);
	vao.bindVertexArrayAttrib(bindingIndex, positionAttrib, GL_FALSE);
	vao.bindVertexArrayAttrib(bindingIndex, normalAttrib, GL_FALSE);
	vao.bindVertexArrayAttrib(bindingIndex, texCoordsAttrib, GL_FALSE);
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
	VertexAttrib texCoordsAttrib = { 1,2,GL_FLOAT, offsetof(QuadVertex, texCoords) };
	unsigned int bindingIndex = 0;
	vao.bindVertexBuffer(bindingIndex, vbo, 0);
	vao.bindVertexArrayAttrib(bindingIndex, positionAttrib, GL_FALSE);
	vao.bindVertexArrayAttrib(bindingIndex, texCoordsAttrib, GL_FALSE);
}

void test_ssao()
{
	// init app
	const int width = 1000, height = 800;
	Application app("ssao", width, height);
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

	// set light
	vec3 lightPosition = glm::vec3(2.0, 4.0, -2.0);
	vec3 lightColor = glm::vec3(0.2, 0.2, 0.7);
	float lightConstant = 1.0f;
	float lightLinear = 0.09f;
	float lightQuadratic = 0.032f;

	// load shader
	ShaderProgram gBufferShader({
		VertexShader::loadFromFile("tests/ssaoTest/shaders/g_buffer.vert").getHandler(),
		FragmentShader::loadFromFile("tests/ssaoTest/shaders/g_buffer.frag").getHandler()
	});
	ShaderProgram ssaoShader({
		VertexShader::loadFromFile("tests/ssaoTest/shaders/ssao.vert").getHandler(),
		FragmentShader::loadFromFile("tests/ssaoTest/shaders/ssao.frag").getHandler()
	});
	ShaderProgram blurShader({
		VertexShader::loadFromFile("tests/ssaoTest/shaders/blur.vert").getHandler(),
		FragmentShader::loadFromFile("tests/ssaoTest/shaders/blur.frag").getHandler()
	});
	ShaderProgram lightingShader({
		VertexShader::loadFromFile("tests/ssaoTest/shaders/lighting.vert").getHandler(),
		FragmentShader::loadFromFile("tests/ssaoTest/shaders/lighting.frag").getHandler()
	});

	// load mesh data
	VertexArray cubeVAO, quadVAO;
	VertexBuffer<CubeVertex> cubeVBO;
	VertexBuffer<QuadVertex> quadVBO;
	initCube(cubeVAO, cubeVBO);
	initQuad(quadVAO, quadVBO);

	string modelPath = "resources/objects/nanosuit/nanosuit.obj";
	Model model(modelPath, MATERIAL_SET::NONE, aiProcess_Triangulate | aiProcess_FlipUVs);
    // mesh data in world position
	mat4 worldMatrix = mat4(1.0f);
	worldMatrix = glm::translate(worldMatrix, glm::vec3(0.0, -1.0f, 0.0f));
	worldMatrix= glm::scale(worldMatrix, glm::vec3(20.0f, 1.0f, 20.0f));
	mat4 cubeWorldMatrix = worldMatrix;
	worldMatrix = mat4(1.0f);
	worldMatrix = glm::translate(worldMatrix, glm::vec3(0.0f, 0.0f, 5.0));
	worldMatrix = glm::rotate(worldMatrix, radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
	worldMatrix = glm::scale(worldMatrix, glm::vec3(0.5f));
	mat4 modelWorldMatrix = worldMatrix;
	
	// init fbo: gBuffer, ssaoFBO, blurFBO
	// gBuffer
	Framebuffer gBufferFBO;
	Renderbuffer gBufferDepth;
	gBufferDepth.allocateStorage(GL_DEPTH_COMPONENT32F, width, height);
	gBufferFBO.attachRenderBuffer(GL_DEPTH_ATTACHMENT, gBufferDepth);
	array<GLenum, 3> attachments = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
	Texture2D gPositionMap;
	gPositionMap.setTexFormat(1, GL_RGB16F, width, height);
	gPositionMap.setTexFilterParameter(GL_NEAREST, GL_NEAREST);
	gPositionMap.setTexWrapParameter(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
	// …Ë÷√ border
	array<float, 3> borderColor = { 0, 0, -1.0f * std::numeric_limits<float>::max() };
	gPositionMap.setTexBorderColor(borderColor.data());
	gBufferFBO.attachTexture2D(attachments[0], gPositionMap);
	Texture2D gNormalMap;
	gNormalMap.setTexFormat(1, GL_RGB16F, width, height);
	gNormalMap.setTexFilterParameter(GL_NEAREST, GL_NEAREST);
	gBufferFBO.attachTexture2D(attachments[1], gNormalMap);
	Texture2D gAlbedoMap;
	gAlbedoMap.setTexFormat(1, GL_RGB16F, width, height);
	gAlbedoMap.setTexFilterParameter(GL_NEAREST, GL_NEAREST);
	gBufferFBO.attachTexture2D(attachments[2], gAlbedoMap);

	gBufferFBO.setColorBuffersToDraw(3, attachments.data());
	if (!gBufferFBO.isComplete())
	{
		std::cout << "g_buffer is not complete!" << std::endl;
		return;
	}
	// ssaoFBO
	Framebuffer ssaoFBO;
	Renderbuffer ssaoDepth;
	ssaoDepth.allocateStorage(GL_DEPTH_COMPONENT32F, width, height);
	ssaoFBO.attachRenderBuffer(GL_DEPTH_ATTACHMENT, ssaoDepth);
	Texture2D ssaoMap;
	ssaoMap.setTexFormat(1, GL_RGB16F, width, height);
	ssaoMap.setTexFilterParameter(GL_NEAREST, GL_NEAREST);
	ssaoFBO.attachTexture2D(GL_COLOR_ATTACHMENT0, ssaoMap);
    if (!ssaoFBO.isComplete())
    {
		std::cout << "ssaoFBO is not complete!" << std::endl;
		return;
    }
    // blurFBO
	Framebuffer blurFBO;
	Renderbuffer blurDepth;
	blurDepth.allocateStorage(GL_DEPTH_COMPONENT32F, width, height);
	blurFBO.attachRenderBuffer(GL_DEPTH_ATTACHMENT, blurDepth);
	Texture2D blurMap;
	blurMap.setTexFormat(1, GL_RGB16F, width, height);
	blurMap.setTexFilterParameter(GL_NEAREST, GL_NEAREST);
	blurFBO.attachTexture2D(GL_COLOR_ATTACHMENT0, blurMap);
	if (!blurFBO.isComplete())
	{
		std::cout << "blurFBO is not complete!" << std::endl;
		return;
	}
	
	// shader: set uniform
	// g_buffer -> vs
	UniformVariable<glm::mat4> gBufferShader_vs_model = gBufferShader.getUniformVariable<glm::mat4>("model");
	UniformVariable<glm::mat4> gBufferShader_vs_view = gBufferShader.getUniformVariable<glm::mat4>("view");
    UniformVariable<glm::mat4> gBufferShader_vs_projection = gBufferShader.getUniformVariable<glm::mat4>("projection");
	// ssao -> fs
	UniformVariable<glm::mat4> ssaoShader_fs_projection = ssaoShader.getUniformVariable<glm::mat4>("projection");
	ssaoShader.setUniformValue("gPositionMap", 0);
	ssaoShader.setUniformValue("gNormalMap", 1);
	ssaoShader.setUniformValue("ssaoNoiseMap", 2);
	// ssao: generate samples
	int sampleCounts = 16;
	float sampleRadius = 0.5f;
	float sampleBias = 0.001f;
	float sampleScaleRadius = 64.0f;
	vector<vec3> samples(sampleCounts);
	uniform_real_distribution<float> floatDistribution(0.0f, 1.0f);
	default_random_engine randomGenerator(sampleCounts);
	for(int i =0; i < sampleCounts;i++)
	{
		float x = floatDistribution(randomGenerator) * 2.0f - 1.0f;
		float y = floatDistribution(randomGenerator) * 2.0f - 1.0f;
		float z = floatDistribution(randomGenerator);
		vec3 sample = vec3(x, y, z);
		float scale = static_cast<float>(i) / static_cast<float>(sampleScaleRadius);
		scale = 0.1f + (1.0f - 0.1f) * (scale * scale);
		sample *= scale;
		samples[i] = sample;
	}
	ssaoShader.setUniformValue("sampleCounts", sampleCounts);
	ssaoShader.setUniformValue("sampleRadius", sampleRadius);
	ssaoShader.setUniformValue("sampleBias", sampleBias);
	for(int i=0; i < sampleCounts; i++)
	{
		ssaoShader.setUniformValue("samples[" + to_string(i) + "]", samples[i]);
	}
	// ssao: generate noise
	int noiseWidth = 4, noiseHeight = 4;
	vector<vec3> ssaoNoise(static_cast<unsigned int>(noiseWidth * noiseHeight));
	vec2 noiseScale = vec2(
		static_cast<float>(width) / static_cast<float>(noiseWidth), 
		static_cast<float>(height) / static_cast<float>(noiseWidth)
	);
	ssaoShader.setUniformValue("noiseScale", noiseScale);
	for(int i=0;i<noiseWidth * noiseHeight;i++)
	{
		float x = floatDistribution(randomGenerator) * 2.0f - 1.0f;
		float y = floatDistribution(randomGenerator) * 2.0f - 1.0f;
		float z = 0.0f;
		ssaoNoise[i] = vec3(x, y, z);
	}
	Texture2D ssaoNoiseMap;
	ssaoNoiseMap.setTexFormat(1, GL_RGB16F, noiseWidth, noiseHeight);
	ssaoNoiseMap.setTexFilterParameter(GL_NEAREST, GL_NEAREST);
	ssaoNoiseMap.setTexWrapParameter(GL_REPEAT, GL_REPEAT);
	ssaoNoiseMap.setTexImageData(noiseWidth, noiseHeight, GL_RGB, GL_FLOAT, ssaoNoise.data());
    
	// blur -> fs
	blurShader.setUniformValue("inputImage", 0);
	// lighting -> fs
	lightingShader.setUniformValue("gPosition", 0);
	lightingShader.setUniformValue("gNormal", 1);
	lightingShader.setUniformValue("gAlbedo", 2);
	lightingShader.setUniformValue("blurMap", 3);

	UniformVariable<vec3> lighting_fs_lightPos = lightingShader.getUniformVariable<vec3>("light.position");
	lightingShader.setUniformValue("light.color", lightColor);
	lightingShader.setUniformValue("light.constant", lightConstant);
	lightingShader.setUniformValue("light.linear", lightLinear);
	lightingShader.setUniformValue("light.quadratic", lightQuadratic);
	
	// openGL config
	glEnable(GL_DEPTH_TEST);

	// render
	while(!glfwWindowShouldClose(window))
	{
		app.getKeyPressInput();
		cameraController.processKeyPressInput();

		// g_buffer pass
		gBufferFBO.bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		gBufferShader.use();
		gBufferShader.setUniformValue(gBufferShader_vs_view, camera.getViewMatrix());
		gBufferShader.setUniformValue(gBufferShader_vs_projection, camera.getProjectionMatrix());
		cubeVAO.bind();
		gBufferShader.setUniformValue(gBufferShader_vs_model, cubeWorldMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		cubeVAO.unbind();
		gBufferShader.setUniformValue(gBufferShader_vs_model, modelWorldMatrix);
		model.draw(gBufferShader, {});
		gBufferShader.unUse();
		gBufferFBO.unbind();

		// ssao pass
		ssaoFBO.bind();
		glClear(GL_DEPTH_BUFFER_BIT);
		ssaoShader.use();
		ssaoShader.setUniformValue(ssaoShader_fs_projection, camera.getProjectionMatrix());
		glActiveTexture(0);
		gPositionMap.bindTexUnit(0);
		glActiveTexture(1);
		gNormalMap.bindTexUnit(1);
		glActiveTexture(2);
		ssaoNoiseMap.bindTexUnit(2);
		quadVAO.bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		quadVAO.unbind();
		ssaoShader.unUse();
		ssaoFBO.unbind();

		// blur pass
		blurFBO.bind();
		glClear(GL_DEPTH_BUFFER_BIT);
		blurShader.use();
		glActiveTexture(0);
		ssaoMap.bindTexUnit(0);
		quadVAO.bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		quadVAO.unbind();
		blurShader.unUse();
		blurFBO.unbind();

		// lighting pass
		glClear(GL_DEPTH_BUFFER_BIT);
		lightingShader.use();
		glActiveTexture(0);
		gPositionMap.bindTexUnit(0);
		glActiveTexture(1);
		gNormalMap.bindTexUnit(1);
		glActiveTexture(2);
		gAlbedoMap.bindTexUnit(2);
		glActiveTexture(3);
		blurMap.bindTexUnit(3);
		vec4 lightPosInView = camera.getViewMatrix() * vec4(lightPosition, 1.0f);
		lightingShader.setUniformValue(lighting_fs_lightPos, vec3(lightPosInView.x, lightPosInView.y, lightPosInView.z));
		quadVAO.bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		quadVAO.unbind();
		lightingShader.unUse();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}
  