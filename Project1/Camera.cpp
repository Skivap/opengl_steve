#include "Camera.h"


#include <iostream>

Camera::Camera(int w, int h){
    this->width = w;
    this->height = h;
}

void Camera::update(GLint view_location) {
    if(shouldRotate)
        this->rotate(0.01f, 0.0f);

    glm::vec3 pos = initPos;
    pos.x = target.x + radius * cos(verticalAngle) * sin(horizontalAngle);
    pos.y = target.y + radius * sin(verticalAngle);
    pos.z = target.z + radius * cos(verticalAngle) * cos(horizontalAngle);

    glm::vec3 dir = glm::normalize(pos - target);
    glm::vec3 right = glm::normalize(glm::cross(up, dir));
    glm::vec3 up = glm::cross(dir, right);

    glm::mat4 view = glm::lookAt(pos, target, up);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
                                            // FOV                                             NEAR / FAR PLANE

    glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(projection * view));
}

void Camera::rotate(float hAngle, float vAngle) {
    horizontalAngle += hAngle;
    verticalAngle += vAngle;

    // Clamp the vertical angle to prevent the camera from flipping over
    verticalAngle = glm::clamp(verticalAngle, -glm::pi<float>() / 2 + 0.1f, glm::pi<float>() / 2 - 0.1f);
}

void Camera::input(GLFWwindow* window) {
    // Keyboard Event
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        radius -= 0.1;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        radius += 0.1;
    }

    // Mouse Event
    static bool left_click = true;
    static double first_x, first_y;
    double cur_x, cur_y;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        shouldRotate = false;

        if (!left_click) {
            glfwGetCursorPos(window, &first_x, &first_y);
            left_click = true;
        }
        else {
            glfwGetCursorPos(window, &cur_x, &cur_y);
            double dx = cur_x - first_x, dy = cur_y - first_y;
            rotate(- dx * sens, dy * sens);
            first_x = cur_x, first_y = cur_y;
        }
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        shouldRotate = true;
        left_click = false;
    }
}

