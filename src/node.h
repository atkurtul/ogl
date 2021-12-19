#pragma once
#include "mesh.h"
#include "physics.h"

struct Node {
  vector<Node*> children;
  vector<Mesh*> meshes;
  unordered_map<Mesh*, vector<btCollisionObject*>> bodies;

  string name;

  glm::mat4 transform;

  virtual void update(Scene* scene, glm::mat4 parent, glm::mat4* uniform);
  virtual void import(struct Scene* scene, const struct aiNode* ainode);
  virtual void render(RenderState&, glm::mat4 const&);

  ~Node();
};
