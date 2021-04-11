#pragma once

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <opengl/opengl.h>
#include "init.h"

using namespace std;
using namespace glm;

struct SkyBoxVertex
{
	glm::vec3 position;
};

struct CubeVertex
{
    glm::vec3 position;
    glm::vec2 uvCoords;
};

inline void testCubeMap1()
{
    const int width = 800, height = 600;
    GLFWwindow* window = initGLFWAndGLAD(width, height);
    if (window == nullptr) return;

    float vertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    vector<CubeVertex> cubeVertices(36);
	for(int i=0, k=0; k<36; i+=5)
	{
        cubeVertices[k++] = { vec3(vertices[i],vertices[i + 1],vertices[i + 2]), vec2(vertices[i + 3],vertices[i + 4]) };
	}

    VertexArray vao;
    VertexBuffer<CubeVertex> vbo;
    vbo.setData(cubeVertices.data(), cubeVertices.size());
    unsigned int bindingIndex = 0;
    VertexAttrib positonAttrib = { 0, 3, GL_FLOAT, offsetof(CubeVertex, position) };
    VertexAttrib uvCoordAttrib = { 1, 2, GL_FLOAT, offsetof(CubeVertex, uvCoords) };
    vao.bindVertexBuffer(bindingIndex, vbo, 0);
    vao.bindVertexArrayAttrib(bindingIndex, positonAttrib, GL_FALSE);
    vao.bindVertexArrayAttrib(bindingIndex, uvCoordAttrib, GL_FALSE);

    Texture2D cubeTexture;
    cubeTexture.loadFromFile("resources/textures/container.jpg", GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE);

    ShaderProgram shaderProgram({
        VertexShader::loadFromFile("tests/test1/shaders/cubeMap.vert").getHandler(),
        FragmentShader::loadFromFile("tests/test1/shaders/cubeMap.frag").getHandler()
    });

    shaderProgram.use();
    mat4 model = mat4(1.0);
    mat4 view = lookAt(vec3(0, 0, 3), vec3(0, 0, 2), vec3(0, 1, 0));
    mat4 projection = perspective(radians(45.0f), static_cast<float>(width) / height, 0.1f, 100.0f);
    shaderProgram.setUniformValue<mat4>("model", model);
	shaderProgram.setUniformValue<mat4>("view", view);
    shaderProgram.setUniformValue<mat4>("projection", projection);
    shaderProgram.setUniformValue<int>("texture0", 0);

    glEnable(GL_DEPTH_TEST);
	
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.use();
        vao.bind();

        glActiveTexture(GL_TEXTURE0);
        cubeTexture.bindTexUnit(0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        vao.unBind();
    	
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    vao.destroy();
    vbo.destroy();
    shaderProgram.destroy();
    glfwTerminate();
}


inline void testCubeMap()
{
    const int width = 800, height = 600;
    GLFWwindow* window = initGLFWAndGLAD(width, height);
    if (window == nullptr) return;

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
    vector<SkyBoxVertex> skyboxVertices(36);
    for (int i = 0, k = 0; k < 36; i += 3)
    {
        skyboxVertices[k++] = { glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]) };
    }

    VertexArray skyboxVao;
    VertexBuffer<SkyBoxVertex> vbo;
    unsigned int bindingIndex = 0;
    VertexAttrib positionAttrib = { 0, 3, GL_FLOAT, offsetof(SkyBoxVertex, position) };
    vbo.setData(skyboxVertices.data(), skyboxVertices.size());
    skyboxVao.bindVertexBuffer<SkyBoxVertex>(bindingIndex, vbo, 0);
    skyboxVao.bindVertexArrayAttrib(bindingIndex, positionAttrib, GL_FALSE);

    CubeMap cubeMap;
    std::array<std::string, 6> filePaths = {
        "resources/textures/skybox/right.jpg",
        "resources/textures/skybox/left.jpg",
        "resources/textures/skybox/top.jpg",
        "resources/textures/skybox/bottom.jpg",
        "resources/textures/skybox/front.jpg",
        "resources/textures/skybox/back.jpg",
    };
    cubeMap.loadFromFiles(filePaths, GL_RGB8, GL_RGB,
        GL_UNSIGNED_BYTE);

    ShaderProgram skyboxShader({
        VertexShader::loadFromFile("tests/test1/shaders/skybox.vert").getHandler(),
        FragmentShader::loadFromFile("tests/test1/shaders/skybox.frag").getHandler()
        });

    skyboxShader.use();
    mat4 view = mat3(lookAt(vec3(0, 0, 3), vec3(0, 0, 2), vec3(0, 1, 0)));
    mat4 projection = perspective(radians(45.0f), static_cast<float>(width) / height, 0.1f, 100.0f);
    skyboxShader.setUniformValue<mat4>("view", view);
    skyboxShader.setUniformValue<mat4>("projection", projection);
    skyboxShader.setUniformValue<int>("skybox", 0);
	
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDepthFunc(GL_LEQUAL);
        skyboxShader.use();
    	
        skyboxVao.bind();
        glActiveTexture(GL_TEXTURE0);
        cubeMap.bindTexUnit(0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        skyboxVao.unBind();
        glDepthFunc(GL_LESS);
    	
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    skyboxVao.destroy();
    vbo.destroy();
    skyboxShader.destroy();

    glfwTerminate();
}