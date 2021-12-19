#pragma once
#include "common.h"

struct Light {
  void import(struct Scene*, const struct aiLight*);

  glm::mat4 prj;
  glm::mat4 view;
  glm::vec3 position;
  glm::vec3 target;
  void create(float zNear,
              float zFar,
              float d,
              glm::vec3 pos,
              glm::vec3 target);

  void update_position(glm::vec3 pos);
};