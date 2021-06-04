#pragma once

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <opengl/opengl.h>
#include <pass/skyboxPass.h>
#include "init.h"

struct SkyBoxVertex
{
	glm::vec3 position;
};

struct CubeVertex
{
    glm::vec3 position;
    glm::vec2 uvCoords;
};

void testCubeMap1();
void testCubeMap();
void testSkyBoxPass();
