#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLfloat f_near_dist = 0.1f;
const GLfloat f_far_dist = 100.0f;

const GLfloat DEF_YAW = -90.0f;
const GLfloat DEF_PITCH = 0.0f;
const GLfloat CAM_SPEED = 2.5f;
const GLfloat CAM_SENS = 0.25f;
const GLfloat CAM_FOV = 70.0f;
const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);
bool first_mouse = true;

GLfloat last_x = (float)WIDTH / 2, last_y = (float)HEIGHT / 2;

class Camera
{

public:
    GLfloat yaw, pitch;
    GLfloat cam_speed, cam_sens, cam_fov;

    glm::vec3 cam_pos, cam_front, cam_right, cam_up;

    Camera() : yaw(DEF_YAW), pitch(DEF_PITCH), cam_speed(CAM_SPEED), cam_sens(CAM_SENS), cam_fov(CAM_FOV)
    {
        cam_pos = glm::vec3(0.0f, 0.0f, 3.0f);
        cam_front = glm::vec3(0.0f, 0.0f, -1.0f);

        updateCameraVectors();
    }

    Camera(GLfloat y, GLfloat p, GLfloat cs, GLfloat csen, GLfloat cf, glm::vec3 cp, glm::vec3 cfr) : yaw(y), pitch(p), cam_speed(cs), cam_sens(csen), cam_fov(cf), cam_pos(cp), cam_front(cfr)
    {
        updateCameraVectors();
    }

    void updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cam_front = glm::normalize(front);
        cam_right = glm::normalize(glm::cross(cam_front, WORLD_UP));
        cam_up = glm::normalize(glm::cross(cam_right, cam_front));
    }

    glm::mat4 getViewMatrix()
    {
        glm::mat4 view_space = glm::lookAt(cam_pos, cam_pos + cam_front, cam_up);
        return view_space;
    }

    glm::mat4 getModelMatrix()
    {
        glm::mat4 model_space = glm::mat4(0.0f);
        return model_space;
    }

    glm::mat4 getProjMatrix()
    {
        glm::mat4 proj_space = glm::perspective(glm::radians(cam_fov), (float)WIDTH / (float)HEIGHT, f_near_dist, f_far_dist);
        return proj_space;
    }

    void processMouseInput(GLfloat x_offset, GLfloat y_offset)
    {
        x_offset *= cam_sens;
        y_offset *= cam_sens;

        yaw += x_offset;
        pitch += y_offset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        updateCameraVectors();
    }

    void processKeyboard(GLfloat deltatime, Window window)
    {
        GLfloat cam_vel = cam_speed * deltatime;

        if (glfwGetKey(window.window, GLFW_KEY_W) == GLFW_PRESS)
            cam_pos += cam_vel * cam_front;

        if (glfwGetKey(window.window, GLFW_KEY_S) == GLFW_PRESS)
            cam_pos -= cam_vel * cam_front;
        if (glfwGetKey(window.window, GLFW_KEY_A) == GLFW_PRESS)
            cam_pos -= cam_vel * cam_right;
        if (glfwGetKey(window.window, GLFW_KEY_D) == GLFW_PRESS)
            cam_pos += cam_vel * cam_right;
    }
};
