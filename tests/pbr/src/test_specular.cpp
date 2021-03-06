#include <app/app.h>
#include <opengl/opengl.h>

#include "test_pbr.h"
#include "pass/skyboxPass.h"

using namespace std;
using namespace glm;

void test_specular()
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

	// init geometry
	Sphere sphere(200);
	Cube cube;
	Quad quad;

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
	Texture2D envMap;
	envMap.loadFromFile("resources/textures/hdr/newport_loft.hdr", 1);
	envMap.setTexWrapParameter(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	envMap.setTexFilterParameter(GL_LINEAR, GL_LINEAR);

	// load shader
	ShaderProgram genEnvCubeMapShader({
		VertexShader::loadFromFile("tests/pbr/shaders/envMap_to_envCubeMap.vert").getHandler(),
		FragmentShader::loadFromFile("tests/pbr/shaders/envMap_to_envCubeMap.frag").getHandler()
	});

	ShaderProgram genPrefilterMapShader({
		VertexShader::loadFromFile("tests/pbr/shaders/envCubeMap_to_prefilterMap.vert").getHandler(),
		FragmentShader::loadFromFile("tests/pbr/shaders/envCubeMap_to_prefilterMap.frag").getHandler()
	});

	ShaderProgram genBRDFlutShader({
		VertexShader::loadFromFile("tests/pbr/shaders/brdfLUT.vert").getHandler(),
		FragmentShader::loadFromFile("tests/pbr/shaders/brdfLUT.frag").getHandler()
	});

	ShaderProgram showBRDFLutShader({
		VertexShader::loadFromFile("tests/pbr/shaders/showBRDFLut.vert").getHandler(),
		FragmentShader::loadFromFile("tests/pbr/shaders/showBRDFLut.frag").getHandler()
	});

	ShaderProgram iblShader({
		VertexShader::loadFromFile("tests/pbr/shaders/ibl_specular.vert").getHandler(),
		FragmentShader::loadFromFile("tests/pbr/shaders/ibl_specular.frag").getHandler()
	});

	// init fbo

	// envCubeFBO
	Framebuffer envCubeMapFBO;
	const int envCubeMapWidth = 1024, envCubeMapHeight = 1024;
	Renderbuffer envCubeMapFBODepth;
	envCubeMapFBODepth.allocateStorage(GL_DEPTH_COMPONENT32F, envCubeMapWidth, envCubeMapHeight);
	envCubeMapFBO.attachRenderBuffer(GL_DEPTH_ATTACHMENT, envCubeMapFBODepth);
	std::shared_ptr<CubeMap> envCubeMap = make_shared<CubeMap>();
	envCubeMap->setTexWrapParameter(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	envCubeMap->setTexFilterParameter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	envCubeMap->setTexFormat(8, GL_RGB16F, envCubeMapWidth, envCubeMapHeight);

	// prefilterMapFBO
	Framebuffer prefilterMapFBO;
    int prefilterMapWidth = 128, prefilterMapHeight = 128;
	int mipLevels = 5;
	Renderbuffer prefilterMapFBODepth;
	std::shared_ptr<CubeMap> prefilterMap = make_shared<CubeMap>();
	prefilterMap->setTexWrapParameter(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	prefilterMap->setTexFilterParameter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	prefilterMap->setTexFormat(mipLevels, GL_RGBA32F, prefilterMapWidth, prefilterMapHeight);
	prefilterMap->genTexMipMap();
	
    // BRDFLutFBO
	Framebuffer brdfLUTFBO;
	const int brdfLUTWidth = 512, brdfLUTHeight = 512;
	Renderbuffer brdfLUTFBODepth;
	brdfLUTFBODepth.allocateStorage(GL_DEPTH_COMPONENT32F, brdfLUTWidth, brdfLUTHeight);
	brdfLUTFBO.attachRenderBuffer(GL_DEPTH_ATTACHMENT, brdfLUTFBODepth);
	std::shared_ptr<Texture2D> brdfLUT = make_shared<Texture2D>();
	brdfLUT->setTexWrapParameter(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	brdfLUT->setTexFilterParameter(GL_LINEAR, GL_LINEAR);
	brdfLUT->setTexFormat(1, GL_RG16F, brdfLUTWidth, brdfLUTHeight);
	brdfLUTFBO.attachTexture2D(GL_COLOR_ATTACHMENT0, *(brdfLUT.get()));

	// add pass
	// SkyBoxPass skyBoxPass(prefilterMap, true);
	SkyBoxPass skyBoxPass(envCubeMap, true);

	// shader set uniform

	// genEnvCubeMapShader --> vs
	UniformVariable<glm::mat4> genEnvCubeMapS_vs_u_mvp = genEnvCubeMapShader.getUniformVariable<glm::mat4>("u_mvp");
	// genEnvCubeMapShader --> fs
	genEnvCubeMapShader.setUniformValue("u_envMap", 0);
	
	// genPrefilterMapShader --> vs
	UniformVariable<glm::mat4> genPrefilterMapS_vs_u_mvp = genPrefilterMapShader.getUniformVariable<glm::mat4>("u_mvp");
	// genPrefilterMapShader --> fs
	UniformVariable<float> genPrefilterMapS_fs_u_roughness = genPrefilterMapShader.getUniformVariable<float>("u_roughness");
	genPrefilterMapShader.setUniformValue("u_envCubeMap", 0);

	// showBRDFLutShader --> fs
	showBRDFLutShader.setUniformValue("u_texture", 0);

	// iblShader --> vs
	UniformVariable<glm::mat4> iblS_vs_model = iblShader.getUniformVariable<glm::mat4>("u_model");
	UniformVariable<glm::mat4> iblS_vs_mvp = iblShader.getUniformVariable<glm::mat4>("u_mvp");
	// iblShader --> fs
	for(int i=0;i<4;i++)
	{
		iblShader.setUniformValue("u_lights[" + to_string(i) + "].position", lightPositions[i]);
		iblShader.setUniformValue("u_lights[" + to_string(i) + "].color", lightColors[i]);
	}
	UniformVariable<vec3> iblS_fs_viewPos = iblShader.getUniformVariable<glm::vec3>("u_viewPos");
	iblShader.setUniformValue("u_albedo", vec3(0.5f, 0.0f, 0.0f));
	iblShader.setUniformValue("u_ao", 1.0f);
	UniformVariable<float> iblS_fs_metallic = iblShader.getUniformVariable<float>("u_metallic");
	UniformVariable<float> iblS_fs_roughness = iblShader.getUniformVariable<float>("u_roughness");
	iblShader.setUniformValue("u_prefilterMap", 0);
	iblShader.setUniformValue("u_brdfLUT", 1);
	
	// openGL config
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	
	// pass1: convert environmentMap to envCubeMap
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
	genEnvCubeMapShader.use();
	glActiveTexture(0);
	envMap.bindTexUnit(0);
	for(int i=0;i<6;i++)
	{
		envCubeMapFBO.attachCubeMapFace(GL_COLOR_ATTACHMENT0, *(envCubeMap.get()), 0, i);
		if (!envCubeMapFBO.isComplete())
		{
			std::cout << " envCubeMapFBO is not complete! " << std::endl;
			return;
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		genEnvCubeMapShader.setUniformValue(genEnvCubeMapS_vs_u_mvp, cubeFaceTransforms[i]);
		cube.draw();
	}
	genEnvCubeMapShader.unUse();
	envCubeMapFBO.unbind();

	// gen envCubeMap mipmap
	envCubeMap->genTexMipMap();

	// pass2 : gen prefilterMap from envCubeMap
	prefilterMapFBO.bind();
	genPrefilterMapShader.use();
	glActiveTexture(0);
	envCubeMap->bindTexUnit(0);
	float mipRatio = 1.0f;
	for(int level =0; level < mipLevels;level++)
	{
		int mipWidth = static_cast<int>(static_cast<float>(prefilterMapWidth) * mipRatio);
	    int mipHeight = static_cast<int>(static_cast<float>(prefilterMapHeight) * mipRatio);
		mipRatio *= 0.5f;
		glViewport(0, 0, mipWidth, mipHeight);
		prefilterMapFBODepth.allocateStorage(GL_DEPTH_COMPONENT32F, mipWidth, mipHeight);
		prefilterMapFBO.attachRenderBuffer(GL_DEPTH_ATTACHMENT, prefilterMapFBODepth);
		// linear
		float roughnessT = static_cast<float>(level) / static_cast<float>(mipLevels - 1);
		genPrefilterMapShader.setUniformValue(genPrefilterMapS_fs_u_roughness, roughnessT);
		for (int face = 0; face < 6; face++)
		{
			prefilterMapFBO.attachCubeMapFace(GL_COLOR_ATTACHMENT0, *(prefilterMap.get()), level, face);
			if (!prefilterMapFBO.isComplete())
			{
				std::cout << " prefilterMapFBO is not complete! " << std::endl;
				return;
			}
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			genPrefilterMapShader.setUniformValue(genPrefilterMapS_vs_u_mvp, cubeFaceTransforms[face]);
			cube.draw();
		}
	}
	genPrefilterMapShader.unUse();
	prefilterMapFBO.unbind();

	// pass3 : gen brdfLUT
	glViewport(0, 0, brdfLUTWidth, brdfLUTHeight);
	brdfLUTFBO.bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	genBRDFlutShader.use();
	quad.draw();
	genBRDFlutShader.unUse();
	brdfLUTFBO.unbind();

	// pass4 : final
	glViewport(0, 0, width, height);
	while(!glfwWindowShouldClose(window))
	{
		app.getKeyPressInput();
		cameraController.processKeyPressInput();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		iblShader.use();
		glActiveTexture(0);
		prefilterMap->bindTexUnit(0);
		glActiveTexture(1);
		brdfLUT->bindTexUnit(1);
		iblShader.setUniformValue(iblS_fs_viewPos, camera.getPosition());
		for(int y =0;y<yNums;y++)
		{
			iblShader.setUniformValue(iblS_fs_metallic, metallic[y]);
			for(int x = 0; x < xNums;x++)
			{
				iblShader.setUniformValue(iblS_fs_roughness, roughness[x]);
				int index = x + y * xNums;
				iblShader.setUniformValue(iblS_vs_model, worldMatrix[index]);
				glm::mat4 mvp = camera.getProjectionMatrix() * camera.getViewMatrix() * worldMatrix[index];
				iblShader.setUniformValue(iblS_vs_mvp, mvp);
				sphere.draw();
			}
		}
		iblShader.unUse();

		skyBoxPass.renderPass(camera.getViewMatrix(), camera.getProjectionMatrix());
		
		// show prefilterMap
		// skyBoxPass.renderPass(camera.getViewMatrix(), camera.getProjectionMatrix());

		// show brdfLut
		/*showBRDFLutShader.use();
		glActiveTexture(0);
		brdfLUT->bindTexUnit(0);
		quad.draw();
		showBRDFLutShader.unUse();*/

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}