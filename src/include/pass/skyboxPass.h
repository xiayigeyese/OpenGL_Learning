#pragma once

#include <vector>
#include <opengl/opengl.h>

class SkyBoxPass
{
	struct SkyBoxVertex
	{
		glm::vec3 position;
	};
public:
	SkyBoxPass()
		:skyboxVao(VertexArray()),
		 vbo(VertexBuffer<SkyBoxVertex>()),
		 shader(ShaderProgram()),
		 cubeMap(CubeMap())
	{ 
	}

	explicit SkyBoxPass(std::array<std::string, 6>& filePaths): SkyBoxPass()
	{
        initSkyBox(filePaths);
	}

    SkyBoxPass(const SkyBoxPass& skyBoxPass) = delete;

	SkyBoxPass(SkyBoxPass&& skyBoxPass) noexcept
        :skyboxVao(std::move(skyBoxPass.skyboxVao)),
	     vbo(std::move(skyBoxPass.vbo)),
		 shader(std::move(skyBoxPass.shader)),
	     cubeMap(std::move(skyBoxPass.cubeMap)),
	     u_vs_view(skyBoxPass.u_vs_view),
		 u_vs_projection(skyBoxPass.u_vs_projection)
	{
	}

    SkyBoxPass& operator=(const SkyBoxPass& skyBoxPass) = delete;
	
    SkyBoxPass& operator=(SkyBoxPass&& skyBoxPass) noexcept
    {
        skyboxVao.operator=(std::move(skyBoxPass.skyboxVao));
        vbo.operator=(std::move(skyBoxPass.vbo));
        shader.operator=(std::move(skyBoxPass.shader));
        cubeMap.operator=(std::move(skyBoxPass.cubeMap));
        u_vs_view = skyBoxPass.u_vs_view;
        u_vs_projection = skyBoxPass.u_vs_projection;
        return *this;
    }

	~SkyBoxPass()
    {
        skyboxVao.destroy();
        vbo.destroy();
        shader.destroy();
        cubeMap.destroy();
    }
	
	void resetSkyBox(std::array<std::string, 6>& filePaths)
    {
        cubeMap.destroy();
        cubeMap.reInitHandler();
        cubeMap.loadFromFiles(filePaths, GL_RGB8, GL_RGB,
            GL_UNSIGNED_BYTE);
    }

	void renderPass(const glm::mat4& view, const glm::mat4& projection)
    {
        glDepthFunc(GL_LEQUAL);
        shader.use();
        shader.setUniformValue(u_vs_view, view);
        shader.setUniformValue(u_vs_projection, projection);
        skyboxVao.bind();
        glActiveTexture(GL_TEXTURE0);
        cubeMap.bindTexUnit(0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        skyboxVao.unBind();
        shader.unUse();
        glDepthFunc(GL_LESS);
    }

private:
    void initSkyBox(std::array<std::string, 6>& filePaths)
    {
        float vertices[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
        };
        std::vector<SkyBoxVertex> skyboxVertices(36);
        for (int i = 0, k = 0; k < 36; i += 3)
        {
            skyboxVertices[k++] = { glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]) };
        }
        
        unsigned int bindingIndex = 0;
        VertexAttrib positionAttrib = { 0, 3, GL_FLOAT, offsetof(SkyBoxVertex, position) };
        vbo.setData(skyboxVertices.data(), skyboxVertices.size());
        skyboxVao.bindVertexBuffer<SkyBoxVertex>(bindingIndex, vbo, 0);
        skyboxVao.bindVertexArrayAttrib(bindingIndex, positionAttrib, GL_FALSE);

        cubeMap.loadFromFiles(filePaths, GL_RGB8, GL_RGB,
            GL_UNSIGNED_BYTE);

        shader.initShaders({
            VertexShader::loadFromFile("src/shaders/skybox.vert").getHandler(),
            FragmentShader::loadFromFile("src/shaders/skybox.frag").getHandler()
            });
        shader.use();
        shader.setUniformValue<int>("skybox", 0);
        u_vs_view = shader.getUniformVariable<glm::mat4>("view");
        u_vs_projection = shader.getUniformVariable<glm::mat4>("projection");
    }

private:
	VertexArray skyboxVao;
    VertexBuffer<SkyBoxVertex> vbo;
	ShaderProgram shader;
	CubeMap cubeMap;
    UniformVariable<glm::mat4> u_vs_view, u_vs_projection;
};
