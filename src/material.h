#pragma once
#include "renderstate.h"
#include "texture.h"


struct Material {
  glm::vec3 diffuse;
  glm::vec3 specular;
  glm::vec3 ambient;
  glm::vec3 emissive;

  Texture* textures[2];

  Material()
      : diffuse(1.f), specular(.5f), ambient(.2f), emissive(0.f), textures{} {}

  void import(struct Scene* scene, const struct aiMaterial* material);

  void bind(RenderState&) const;
};
