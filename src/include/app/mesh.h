#pragma once

#include <vector>
#include <string>
#include <memory>

#include <glm/glm.hpp>
#include <opengl/opengl.h>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};


enum class VERTEX_ATTRIBUTE
{
	P_N_UV,          // P: position, N: normal, UV: texCoords      
	P_N_UV_T_B       // T: tangent, B: bitangent
};


enum class TEXTURE_TYPE
{
	DIFFUSE,
	SPECULAR,
	NORMAL,
	HEIGHT
};

// one material in model use for many mesh, so mesh has texture pointer
struct Material
{
	std::shared_ptr<Texture2D> texture;
	TEXTURE_TYPE textureType = TEXTURE_TYPE::DIFFUSE;
	std::string path;
};


enum class MATERIAL_SET
{
	NONE,           // none map
	D,              // D: diffuseMap
	D_S,            // S: specular
	// D_S_N,          // N: normalMap
	D_S_N_H         // H: heightMap
};

class Mesh
{
public:
	explicit Mesh(const std::vector<Vertex> vertices, const std::vector<unsigned int> indices)
		:m_vertices(vertices), m_indices(indices)
	{
		initVAO();
	}
	

	void setMaterials(std::vector<Material> materials, std::vector<unsigned int> texUnits)
	{
		m_materials = materials;
		m_texUnits = texUnits;
	}

	void draw(const ShaderProgram& shader)
	{
		for(unsigned int i=0;i<m_materials.size();i++)
		{
			glActiveTexture(GL_TEXTURE0 + m_texUnits[i]);
			m_materials[0].texture->bindTexUnit(m_texUnits[i]);
		}	
		m_vao.bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, nullptr);
		m_vao.unbind();
	}


private:
	void initVAO()
	{
		// vao bind vbo
		m_vbo.setData(m_vertices.data(), m_vertices.size());
		VertexAttrib positionAttrib = { 0, 3, GL_FLOAT, offsetof(Vertex, position) };
		VertexAttrib normalAttrib = { 1, 3, GL_FLOAT, offsetof(Vertex, normal) };
		VertexAttrib texCoordsAttrib = { 2, 2, GL_FLOAT, offsetof(Vertex, texCoords) };
		unsigned int bindingIndex = 0;
		m_vao.bindVertexBuffer(bindingIndex, m_vbo, 0);
		m_vao.bindVertexArrayAttrib(bindingIndex, positionAttrib);
		m_vao.bindVertexArrayAttrib(bindingIndex, normalAttrib);
		m_vao.bindVertexArrayAttrib(bindingIndex, texCoordsAttrib);
		// vao bind ebo
		m_ebo.setData(m_indices.data(), m_indices.size() * sizeof(unsigned int));
		m_vao.bindElementBuffer(m_ebo);
	}

private:
	VertexArray m_vao;
	VertexBuffer<Vertex> m_vbo;
	Buffer m_ebo;
	
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<Material> m_materials;
	std::vector<unsigned int> m_texUnits;
};
