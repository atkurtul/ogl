#pragma once
#include "common.h"
#include "shader.h"

struct RenderState {
  double dt;
  glm::mat4 transform;
  Shader* active_shader;
};