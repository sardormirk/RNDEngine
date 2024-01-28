#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shaders.h"
#include "Window.h"
#include "Camera.h"
#include <Lights.h>
#include <iostream>

const char *vShaderPath = "/Users/voidblob/Desktop/OpenGL_dependencies/Shaders/shader.vs";
const char *fShaderPath = "/Users/voidblob/Desktop/OpenGL_dependencies/Shaders/shader.fs";

const char *lvShaderPath = "/Users/voidblob/Desktop/OpenGL_dependencies/Shaders/lShader.vs";
const char *lfShaderPath = "/Users/voidblob/Desktop/OpenGL_dependencies/Shaders/lShader.fs";

void mouse_callback(GLFWwindow *window, GLdouble x_pos, GLdouble y_pos);
GLint process_init(Window window);
GLint process_terminate();

GLfloat delta_time = 0.0f;
GLfloat frame_time = 0.0f;

Camera main_cam;

int main()
{

    Window window("Window");
    process_init(window);

    Shader shader(vShaderPath, fShaderPath);
    Shader lShader(lvShaderPath, lfShaderPath);

    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f};

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)};

    GLuint VBO, lVAO, VAO;

    glGenVertexArrays(1, &VAO);
    glGenVertexArrays(1, &lVAO);

    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(lVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(3);

    glm::mat4 model = main_cam.getModelMatrix();
    glm::mat4 proj = main_cam.getProjMatrix();

    glm::vec3 cubeColor = glm::vec3(0.23f, 0.82f, 0.82f);
    glm::vec3 cubeSpec = glm::vec3(0.5f);
    glm::vec3 lightColor = glm::vec3(1.0f);
    glm::vec3 lightPosition = glm::vec3(1.0f, 1.0f, 2.0f);

    DirLight dirLight;
    PointLight pointLight;

    while (!window.shouldClose())
    {

        delta_time = glfwGetTime() - frame_time;
        frame_time = glfwGetTime();

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = main_cam.getViewMatrix();

        shader.use();
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("proj", proj);

        shader.setVec3("material.ambient", cubeColor);
        shader.setVec3("material.diffuse", cubeColor);
        shader.setVec3("material.specular", cubeSpec);
        shader.setFloat("material.shininess", 32.0f);

        shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

        shader.setVec3("pointLights[0].position", lightPosition);
        shader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        shader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        shader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        shader.setFloat("pointLights[0].constant", 1.0f);
        shader.setFloat("pointLights[0].linear", 0.09f);
        shader.setFloat("pointLights[0].quadratic", 0.032f);

        shader.setVec3("viewPos", main_cam.cam_pos);

        glBindVertexArray(VAO);

        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        lShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPosition);
        model = glm::scale(model, glm::vec3(0.5f));

        lShader.setMat4("model", model);
        lShader.setMat4("view", view);
        lShader.setMat4("proj", proj);
        lShader.setVec4("lightColor", lightColor.x, lightColor.y, lightColor.z, 1.0f);

        glBindVertexArray(lVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        window.swapBufs();
        window.processInput();
        main_cam.processKeyboard(delta_time, window);
    }

    window.deleteWindow();
    process_terminate();
}

GLint process_init(Window window)
{
    glfwSetCursorPosCallback(window.window, mouse_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    return 1;
}

GLint process_terminate()
{
    glfwTerminate();
    return 0;
}

void mouse_callback(GLFWwindow *window, GLdouble x_pos, GLdouble y_pos)
{
    if (first_mouse)
    {
        last_x = x_pos;
        last_y = y_pos;
        first_mouse = false;
    }

    GLfloat x_offset = x_pos - last_x;
    GLfloat y_offset = last_y - y_pos;

    last_x = x_pos;
    last_y = y_pos;

    main_cam.processMouseInput(x_offset, y_offset);
}