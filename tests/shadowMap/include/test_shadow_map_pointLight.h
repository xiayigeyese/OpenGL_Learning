#pragma once

#include <glm/glm.hpp>

#include <opengl/opengl.h>

struct CubeVertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uvCoords;
};

CubeMap createCubeShadowMap(const int width, const int height, const GLenum internalFormat);

void initCubeVAO(VertexArray&, VertexBuffer<CubeVertex>&);

void testShadowMapPointLight();
