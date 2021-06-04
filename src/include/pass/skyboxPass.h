#pragma once
#include <memory>
#include <opengl/opengl.h>

class SkyBoxPass
{
	struct SkyBoxVertex
	{
		glm::vec3 position;
	};
public:
	SkyBoxPass()
		:m_skyBoxVao(VertexArray()),
	     m_vbo(VertexBuffer<SkyBoxVertex>()),
		 m_shader(ShaderProgram())
	{
		
	}

	explicit SkyBoxPass(const std::shared_ptr<CubeMap>& cubeMapPtr, bool isHDR = false): SkyBoxPass()
	{
		initSkyBox(cubeMapPtr, isHDR);
	}
	

    SkyBoxPass(const SkyBoxPass& skyBoxPass) = delete;

	SkyBoxPass(SkyBoxPass&& skyBoxPass) noexcept
        :m_skyBoxVao(std::move(skyBoxPass.m_skyBoxVao)),
		 m_vbo(std::move(skyBoxPass.m_vbo)),
		 m_shader(std::move(skyBoxPass.m_shader)),
	     m_cubeMapPtr(std::move(skyBoxPass.m_cubeMapPtr)),
	     u_vs_view(skyBoxPass.u_vs_view),
		 u_vs_projection(skyBoxPass.u_vs_projection)
	{
	}

    SkyBoxPass& operator=(const SkyBoxPass& skyBoxPass) = delete;
	
    SkyBoxPass& operator=(SkyBoxPass&& skyBoxPass) noexcept
    {
        m_skyBoxVao.operator=(std::move(skyBoxPass.m_skyBoxVao));
		m_vbo.operator=(std::move(skyBoxPass.m_vbo));
        m_shader.operator=(std::move(skyBoxPass.m_shader));
        m_cubeMapPtr = skyBoxPass.m_cubeMapPtr;
        u_vs_view = skyBoxPass.u_vs_view;
        u_vs_projection = skyBoxPass.u_vs_projection;
        return *this;
    }

	~SkyBoxPass()
    {
        m_skyBoxVao.destroy();
        m_shader.destroy();
    }
	
	void resetSkyBox(const std::shared_ptr<CubeMap>& cubeMapPtr)
    {
		m_cubeMapPtr = cubeMapPtr;
    }

	void renderPass(const glm::mat4& view, const glm::mat4& projection)
    {
        glDepthFunc(GL_LEQUAL);
        m_shader.use();
        m_shader.setUniformValue(u_vs_view, glm::mat4(glm::mat3(view)));
        m_shader.setUniformValue(u_vs_projection, projection);
        m_skyBoxVao.bind();
        glActiveTexture(GL_TEXTURE0);
        m_cubeMapPtr->bindTexUnit(0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        m_skyBoxVao.unbind();
        m_shader.unUse();
        glDepthFunc(GL_LESS);
    }

private:
    void initSkyBox(const std::shared_ptr<CubeMap>& cubeMapPtr, bool isHDR)
    {
        std::array<SkyBoxVertex, 36> skyboxVertices =
        {
			SkyBoxVertex{{-1.0f,  1.0f, -1.0f}},
			SkyBoxVertex{{-1.0f, -1.0f, -1.0f}},
			SkyBoxVertex{{ 1.0f, -1.0f, -1.0f}},
			SkyBoxVertex{{ 1.0f, -1.0f, -1.0f}},
			SkyBoxVertex{{ 1.0f,  1.0f, -1.0f}},
			SkyBoxVertex{{-1.0f,  1.0f, -1.0f}},
			SkyBoxVertex{{-1.0f, -1.0f,  1.0f}},
			SkyBoxVertex{{-1.0f, -1.0f, -1.0f}},
			SkyBoxVertex{{-1.0f,  1.0f, -1.0f}},
			SkyBoxVertex{{-1.0f,  1.0f, -1.0f}},
			SkyBoxVertex{{-1.0f,  1.0f,  1.0f}},
			SkyBoxVertex{{-1.0f, -1.0f,  1.0f}},
			SkyBoxVertex{{ 1.0f, -1.0f, -1.0f}},
			SkyBoxVertex{{ 1.0f, -1.0f,  1.0f}},
			SkyBoxVertex{{ 1.0f,  1.0f,  1.0f}},
			SkyBoxVertex{{ 1.0f,  1.0f,  1.0f}},
			SkyBoxVertex{{ 1.0f,  1.0f, -1.0f}},
			SkyBoxVertex{{ 1.0f, -1.0f, -1.0f}},
			SkyBoxVertex{{-1.0f, -1.0f,  1.0f}},
			SkyBoxVertex{{-1.0f,  1.0f,  1.0f}},
			SkyBoxVertex{{ 1.0f,  1.0f,  1.0f}},
			SkyBoxVertex{{ 1.0f,  1.0f,  1.0f}},
			SkyBoxVertex{{ 1.0f, -1.0f,  1.0f}},
			SkyBoxVertex{{-1.0f, -1.0f,  1.0f}},
			SkyBoxVertex{{-1.0f,  1.0f, -1.0f}},
			SkyBoxVertex{{ 1.0f,  1.0f, -1.0f}},
			SkyBoxVertex{{ 1.0f,  1.0f,  1.0f}},
			SkyBoxVertex{{ 1.0f,  1.0f,  1.0f}},
			SkyBoxVertex{{-1.0f,  1.0f,  1.0f}},
			SkyBoxVertex{{-1.0f,  1.0f, -1.0f}},
			SkyBoxVertex{{-1.0f, -1.0f, -1.0f}},
			SkyBoxVertex{{-1.0f, -1.0f,  1.0f}},
			SkyBoxVertex{{ 1.0f, -1.0f, -1.0f}},
			SkyBoxVertex{{ 1.0f, -1.0f, -1.0f}},
			SkyBoxVertex{{-1.0f, -1.0f,  1.0f}},
			SkyBoxVertex{{ 1.0f, -1.0f,  1.0f}}
        };
        
        
        unsigned int bindingIndex = 0;
        VertexAttrib positionAttrib = { 0, 3, GL_FLOAT, offsetof(SkyBoxVertex, position) };
		m_vbo.setData(skyboxVertices.data(), skyboxVertices.size());
        m_skyBoxVao.bindVertexBuffer<SkyBoxVertex>(bindingIndex, m_vbo, 0);
        m_skyBoxVao.bindVertexArrayAttrib(bindingIndex, positionAttrib, GL_FALSE);

		m_cubeMapPtr = cubeMapPtr;
    	if (isHDR)
    	{
			m_shader.initShaders({
				VertexShader::loadFromFile("src/shaders/skybox/skybox.vert").getHandler(),
				FragmentShader::loadFromFile("src/shaders/skybox/skybox_hdr.frag").getHandler()
			});
    	}
    	else
		{
			m_shader.initShaders({
				VertexShader::loadFromFile("src/shaders/skybox/skybox.vert").getHandler(),
				FragmentShader::loadFromFile("src/shaders/skybox/skybox.frag").getHandler()
			});
		}
        
        m_shader.use();
        m_shader.setUniformValue<int>("skybox", 0);
        u_vs_view = m_shader.getUniformVariable<glm::mat4>("u_view");
        u_vs_projection = m_shader.getUniformVariable<glm::mat4>("u_projection");
    }

private:
	VertexArray m_skyBoxVao;
	VertexBuffer<SkyBoxVertex> m_vbo;
	ShaderProgram m_shader;
	std::shared_ptr<CubeMap> m_cubeMapPtr;
	
    UniformVariable<glm::mat4> u_vs_view, u_vs_projection;
};
