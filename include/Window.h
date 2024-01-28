#pragma once
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <string>
#include <iostream>

const GLint WIDTH = 800;
const GLint HEIGHT = 600;

static void glfwError(int id, const char *description)
{
    std::cout << description << std::endl;
}

void framebuffer_size_callback(GLFWwindow *window, GLint width, GLint height)
{
    glViewport(0, 0, width, height);
}

class Window
{
public:
    GLFWwindow *window = NULL;

    Window(GLint width, GLint height, const char *name)
    {
        init();
        window = glfwCreateWindow(width, height, name, NULL, NULL);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        checkErr();
        setWindow();
    }

    Window(const char *NAME)
    {
        init();
        window = glfwCreateWindow(WIDTH, HEIGHT, NAME, NULL, NULL);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        checkErr();
        setWindow();
    }

    void setWindow()
    {
        glfwMakeContextCurrent(window);
    }

    void deleteWindow()
    {
        glfwDestroyWindow(window);
    }

    void swapBufs()
    {
        glfwSwapBuffers(window);
    }

    void processInput()
    {
        glfwPollEvents();
        }

    GLint shouldClose()
    {
        return glfwWindowShouldClose(window);
    }

private:
    GLint checkErr()
    {
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }
    }

    void init()
    {
        glfwSetErrorCallback(&glfwError);
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }
};