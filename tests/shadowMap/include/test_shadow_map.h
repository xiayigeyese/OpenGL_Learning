#pragma once

#include <glm/glm.hpp>

#include <opengl/opengl.h>

struct BackVertex
{
	glm::vec3 position;
	glm::vec2 uvCoords;
};

struct CubeVertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uvCoords;
};

using PlaneVertex = CubeVertex;

Texture2D createShadowMap(const int width, const int height, const GLenum internalFormat);

void initBackVAO(VertexArray& backVAO, VertexBuffer<BackVertex>& vbo);

void initCubeVAO(VertexArray& cubeVAO, VertexBuffer<CubeVertex>& vbo);

void initPlaneVAO(VertexArray& planeVAO, VertexBuffer<PlaneVertex>& vbo);

void testDepthMap();

void testShadowMap();