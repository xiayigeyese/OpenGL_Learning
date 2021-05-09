#pragma once

#include <glm/glm.hpp>
#include <opengl/opengl.h>

struct CubeVertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct QuadVertex
{
	glm::vec3 position;
	glm::vec2 texCoords;
};

void initCube(VertexArray&, VertexBuffer<CubeVertex>&);
void initQuad(VertexArray&, VertexBuffer<QuadVertex>&);

void test_ssao();