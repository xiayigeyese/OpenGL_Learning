#pragma once

#include <glm/glm.hpp>

#include <opengl/opengl.h>

struct CubeVertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uvCoords;
};

void initCubeVAO(VertexArray&, VertexBuffer<CubeVertex>&);

void testShadowMapPointLight();
