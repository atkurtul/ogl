#pragma once
#include "common.h"

struct Shader {
    uint id;

    void load(string vs, string ps);

    void bind();

    void set_uniform(const char* name, bool data);

    void set_uniform_array(const char* name, const glm::mat4* data, uint size);
    void set_uniform(const char* name, glm::mat4 const& data);
    void set_uniform(const char* name, glm::vec3 const& data);
    void set_uniform(const char* name, glm::vec4 const& data);
    void set_viewport(int w, int h, int x, int y);
};