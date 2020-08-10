#pragma once
#include "node.h"
#include "window.h"

struct Camera : Node {
    glm::mat4 prj;
    glm::mat3 rotation;
    glm::vec3 position;
    Camera() = default;

    Camera(float fov, float aspect_ratio) : position(0, 4, 0) {
        prj = glm::perspective(glm::radians(fov), aspect_ratio, 0.01f, 4000.f);
        rotation = glm::identity<glm::mat3>();
    }

    Camera(float fov, float aspect_ratio, glm::vec3 pos, glm::vec3 target) : position(pos) {
        prj = glm::perspective(glm::radians(fov), aspect_ratio, 0.01f, 4000.f);
        rotation = glm::lookAt(pos, target, glm::vec3(0, 1, 0));
    }

    glm::mat4 get_view_prj() {
        glm::mat4 view = rotation;
        view[3] = glm::vec4(rotation * -position, 1);
        return  prj * view;
    }

    glm::mat4 update(Window const& win) {
        const float speed = win.dt * 40;

        auto tmp = glm::transpose(rotation);
        position -= tmp[2] * (speed * win.get_key(GLFW_KEY_W));
        position += tmp[2] * (speed * win.get_key(GLFW_KEY_S));
        position -= tmp[0] * (speed * win.get_key(GLFW_KEY_A));
        position += tmp[0] * (speed * win.get_key(GLFW_KEY_D));

        const auto up = glm::vec3(0, 1, 0);
        auto mx = glm::mat3(glm::angleAxis(win.mdelta.x * 0.0025f, up));
        auto my = glm::mat3(glm::angleAxis(win.mdelta.y * 0.0025f, tmp[0]));
        rotation *= my * mx;
        glm::mat4 view = rotation;
        view[3] = glm::vec4(rotation * -position, 1);
        return  prj * view;
    }
};
