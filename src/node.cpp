#include "node.h"
#include "assimp/scene.h"
#include "scene.h"

void Node::update(Scene* scene, glm::mat4 parent, glm::mat4* uniform) {
  for (auto child : children) {
    child->update(scene, parent, uniform);
  }
}

void Node::import(Scene* scene, const aiNode* ainode) {
  name = ainode->mName.C_Str();

  memcpy(&transform, &ainode->mTransformation, 64);
  transform = glm::transpose(transform);

  for (uint i = 0; i < ainode->mNumMeshes; ++i) {
    auto mesh = scene->meshes[ainode->mMeshes[i]];
    meshes.push_back(mesh);
  }

  for (unsigned int i = 0; i < ainode->mNumChildren; ++i) {
    auto c = ainode->mChildren[i];
    Node* child;
    if (scene->bonemap.find(c->mName.C_Str()) != scene->bonemap.end()) {
      child = new Bone;
    } else
      child = new Node;
    child->import(scene, c);
    children.push_back(child);
  }
}

void Node::render(RenderState& state, glm::mat4 const& parent_transform) {
  glm::mat4 final = parent_transform * transform;
  state.active_shader->set_uniform("xform", final);

  for (auto mesh : meshes) {
    if (bodies.empty())
      mesh->render(state);
    else
      for (auto body : bodies[mesh]) {
        glm::mat4 m(1.f);
        body->getWorldTransform().getOpenGLMatrix(&m[0][0]);
        state.active_shader->set_uniform("xform", final * m);
        mesh->render(state);
      }
  }

  for (auto child : children) {
    child->render(state, final);
  }
}

Node::~Node() {
  for (auto node : children) {
    delete node;
  }
}