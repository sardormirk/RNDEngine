#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shaders.h"
#include "Window.h"
#include "Camera.h"
#include <Lights.h>
#include <iostream>
#include <imgui.h>
#include <Model.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

const char *vShaderPath = "RNDEngine/Shaders/shader.vs";
const char *fShaderPath = "RNDEngine / Shaders / shader.fs";

const char *lvShaderPath = "RNDEngine/Shaders/lShader.vs";
const char *lfShaderPath = "RNDEngine/Shaders/lShader.fs";

const char* modelvShaderPath = "RNDEngine/Shaders/model_shader.vs";
const char* modelfShaderPath = "RNDEngine/Shaders/model_shader.fs";

const string modelPath = "RNDEngine/resources/backpack/backpack.obj";

void mouse_callback(GLFWwindow *window, GLdouble x_pos, GLdouble y_pos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

GLint process_init(Window window);
GLint process_terminate();

GLfloat delta_time = 0.0f;
GLfloat frame_time = 0.0f;

bool cursorDisabled = false;
bool showMenu = true;
bool lightOn = false;

Camera main_cam;

unsigned int loadCubemap(vector<std::string> faces);


int main()
{

    Window window(2000, 1500, "Window");
    process_init(window);

    stbi_set_flip_vertically_on_load(true);

    Shader shader(vShaderPath, fShaderPath);
    Shader lShader(lvShaderPath, lfShaderPath);
    Shader modelShader(modelvShaderPath, modelfShaderPath);
    Shader skyboxShader("RNDEngine/Shaders/skybox.vs", "RNDEngine/Shaders/skybox.fs");

    Model modelBackpack(modelPath);


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplGlfw_InitForOpenGL(window.window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();


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

    GLfloat skyboxVertices[] = {
        // positions          
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

    GLuint skyboxVAO, skyboxVBO;

    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);

    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    vector<std::string> faces
    {
        "RNDEngine/resources/skybox/skybox/right.jpg",
        "RNDEngine/resources/skybox/skybox/left.jpg",
        "RNDEngine/resources/skybox/skybox/top.jpg",
        "RNDEngine/resources/skybox/skybox/bottom.jpg",
        "RNDEngine/resources/skybox/skybox/front.jpg",
        "RNDEngine/resources/skybox/skybox/back.jpg"

    };

    stbi_set_flip_vertically_on_load(false);
    unsigned int cubemapTexture = loadCubemap(faces);

    glm::mat4 model = main_cam.getModelMatrix();
    glm::mat4 proj = main_cam.getProjMatrix();

    //Cube data
    glm::vec3 cubeColor = glm::vec3(0.23f, 0.82f, 0.82f);
    glm::vec3 cubeSpec = glm::vec3(1.0f);
    glm::vec3 cubePos = glm::vec3(1.0f);
    GLfloat cubeShininess = 32;


    glm::vec3 lightColor = glm::vec3(1.0f);
    glm::vec3 lightPosition = glm::vec3(1.0f, 1.0f, 2.0f);

    DirLight dirLight;
    PointLight pointLight;

    while (!window.shouldClose())
    {

        delta_time = glfwGetTime() - frame_time;
        frame_time = glfwGetTime();

        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        
        if (showMenu) {
            ImGui::Begin("Engine");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::Separator();
            ImGui::ColorEdit3("Light Color", (float*)&lightColor);
            ImGui::Checkbox("Light On", &lightOn);
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 10.0f));
            ImGui::Text("Light Position");
            ImGui::DragFloat("X_Pos", (float*)&lightPosition.x, 0.1f);
            ImGui::DragFloat("Y_Pos", (float*)&lightPosition.y, 0.1f);
            ImGui::DragFloat("Z_Pos", (float*)&lightPosition.z, 0.1f);
            ImGui::End();
        }
        

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = main_cam.getViewMatrix();
        modelShader.use();

        modelShader.setMat4("projection", proj);
        modelShader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

        modelShader.setMat4("model", model);
  
        modelShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        modelShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        modelShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        modelShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

        modelShader.setVec3("pointLights[0].position", lightPosition);
        modelShader.setVec3("pointLights[0].ambient", lightColor.x * 0.1, lightColor.y * 0.1, lightColor.z * 0.1);
        modelShader.setVec3("pointLights[0].diffuse", lightColor);
        modelShader.setVec3("pointLights[0].specular", lightColor);
        modelShader.setFloat("pointLights[0].constant", 1.0f);
        modelShader.setFloat("pointLights[0].linear", 0.09f);
        modelShader.setFloat("pointLights[0].quadratic", 0.032f);
        modelShader.setVec3("viewPos", main_cam.cam_pos);

        if (!lightOn) {
            modelShader.setBool("lightsOn", 0);
        }
        else {
            modelShader.setBool("lightsOn", 1);
        }

        modelBackpack.Draw(modelShader);

        lShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPosition);
        model = glm::scale(model, glm::vec3(0.5f));

        lShader.setMat4("model", model);
        lShader.setMat4("view", view);
        lShader.setMat4("proj", proj);
        lShader.setVec4("lightColor", lightColor.x, lightColor.y, lightColor.z, 1.0f);

        if (lightOn) {
            glBindVertexArray(lVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glDepthFunc(GL_LEQUAL);  
        skyboxShader.use();
        view = glm::mat4(glm::mat3(main_cam.getViewMatrix())); 
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", proj);

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
        

       /* shader.use();

        Cube rendering code
        model = glm::mat4(1.0f);
        model = glm::translate(model, cubePos);

        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("proj", proj);

        shader.setVec3("material.ambient", cubeColor);
        shader.setVec3("material.diffuse", cubeColor);
        shader.setVec3("material.specular", cubeSpec);
        shader.setFloat("material.shininess", cubeShininess);

        shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

        shader.setVec3("pointLights[0].position", lightPosition);
        shader.setVec3("pointLights[0].ambient", lightColor.x * 0.1, lightColor.y * 0.1, lightColor.z * 0.1);
        shader.setVec3("pointLights[0].diffuse", lightColor);
        shader.setVec3("pointLights[0].specular", lightColor);
        shader.setFloat("pointLights[0].constant", 1.0f);
        shader.setFloat("pointLights[0].linear", 0.09f);
        shader.setFloat("pointLights[0].quadratic", 0.032f);

        shader.setVec3("viewPos", main_cam.cam_pos);

        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        

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


        */



        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window.swapBufs();
        window.processInput();
        main_cam.processKeyboard(delta_time, window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    window.deleteWindow();
    process_terminate();
}

GLint process_init(Window window)
{
    glfwSetCursorPosCallback(window.window, mouse_callback);
    glfwSetKeyCallback(window.window, key_callback);

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

    if (cursorDisabled) {
        x_offset = x_pos - last_x;
        y_offset = last_y - y_pos;

    }
    else {
        x_offset = 0;
        y_offset = 0;
    }

    last_x = x_pos;
    last_y = y_pos;

    main_cam.processMouseInput(x_offset, y_offset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_I && action == GLFW_PRESS) {
        if (cursorDisabled == false) {
            cursorDisabled = true;
            showMenu = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else {
            cursorDisabled = false;
            showMenu = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}


unsigned int loadCubemap(vector<std::string> faces)
{
   

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}