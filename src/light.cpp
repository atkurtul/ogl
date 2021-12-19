#include "light.h"
#include "assimp/scene.h"
#include "scene.h"


void Light::import(Scene*, const aiLight* light) {}

void Light::create(float zNear,
                   float zFar,
                   float d,
                   glm::vec3 pos,
                   glm::vec3 tar) {
  prj = glm::ortho(-d, d, -d, d, zNear, zFar);
  position = pos;
  target = tar;
  view = glm::lookAt(pos, target, glm::vec3(0, 1, 0));
}

void Light::update_position(glm::vec3 pos) {
  position = pos;
  view = glm::lookAt(pos, target, glm::vec3(0, 1, 0));
}
