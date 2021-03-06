#include <app/app.h>
#include "test_cubeMap.h"

void testCubeMap1()
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

    std::vector<CubeVertex> cubeVertices(36);
    for (int i = 0, k = 0; k < 36; i += 5)
    {
        cubeVertices[k++] = { glm::vec3(vertices[i],vertices[i + 1],vertices[i + 2]), glm::vec2(vertices[i + 3],vertices[i + 4]) };
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
    cubeTexture.loadFromFile("resources/textures/container.jpg");

    ShaderProgram shaderProgram({
        VertexShader::loadFromFile("tests/cubeMap/shaders/cubeMap.vert").getHandler(),
        FragmentShader::loadFromFile("tests/cubeMap/shaders/cubeMap.frag").getHandler()
        });

    shaderProgram.use();
    glm::mat4 model = glm::mat4(1.0);
    glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 2), glm::vec3(0, 1, 0));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / height, 0.1f, 100.0f);
    shaderProgram.setUniformValue<glm::mat4>("model", model);
    shaderProgram.setUniformValue<glm::mat4>("view", view);
    shaderProgram.setUniformValue<glm::mat4>("projection", projection);
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
        vao.unbind();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    vao.destroy();
    vbo.destroy();
    shaderProgram.destroy();
    glfwTerminate();
}

void testCubeMap()
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
    std::vector<SkyBoxVertex> skyboxVertices(36);
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
        VertexShader::loadFromFile("tests/cubeMap/shaders/skybox.vert").getHandler(),
        FragmentShader::loadFromFile("tests/cubeMap/shaders/skybox.frag").getHandler()
        });

    skyboxShader.use();
    UniformVariable<glm::mat4> u_vs_view = skyboxShader.getUniformVariable<glm::mat4>("view");
    UniformVariable<glm::mat4> u_vs_projection = skyboxShader.getUniformVariable<glm::mat4>("projection");
    skyboxShader.setUniformValue<int>("skybox", 0);

    glEnable(GL_DEPTH_TEST);
    int theta = 0;

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDepthFunc(GL_LEQUAL);
        skyboxShader.use();
        theta = theta % 360000;
        theta++;
        glm::mat4 view = glm::mat3(glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(sin(theta * 0.001), 0, 3 - cos(theta * 0.001)), glm::vec3(0, 1, 0)));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / height, 0.1f, 100.0f);
        skyboxShader.setUniformValue(u_vs_view, view);
        skyboxShader.setUniformValue(u_vs_projection, projection);
        skyboxVao.bind();
        glActiveTexture(GL_TEXTURE0);
        cubeMap.bindTexUnit(0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        skyboxVao.unbind();
        glDepthFunc(GL_LESS);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    skyboxVao.destroy();
    vbo.destroy();
    skyboxShader.destroy();

    glfwTerminate();
}

void testSkyBoxPass()
{
	
    const int width = 800, height = 600;
    Application app("skybox", width, height);
    GLFWwindow* window = app.getWindow();
    Input* input = app.getInput();

    Camera camera = Camera::perspectiveCamera(
        glm::vec3(0, 0, 0),
        glm::vec3(0, 0, -1),
        glm::vec3(0,1,0),
        45.0f,
        static_cast<float>(width) / height,
        0.1f,
        100.0f
    );
    CameraController cameraController(&camera, input);

    std::array<std::string, 6> filePaths = {
        "resources/textures/skybox/right.jpg",
        "resources/textures/skybox/left.jpg",
        "resources/textures/skybox/top.jpg",
        "resources/textures/skybox/bottom.jpg",
        "resources/textures/skybox/front.jpg",
        "resources/textures/skybox/back.jpg",
    };

    std::shared_ptr<CubeMap> cubeMap = std::make_shared<CubeMap>();
    cubeMap->loadFromFiles(filePaths, GL_RGB8, GL_RGB,
        GL_UNSIGNED_BYTE);

    SkyBoxPass skyBoxPass(cubeMap);

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {
        app.getKeyPressInput();
        cameraController.processKeyPressInput();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        skyBoxPass.renderPass(camera.getViewMatrix(), camera.getProjectionMatrix());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}