#include "glad/glad.h"

#include "assimp/scene.h"
#include "scene.h"

glm::vec3 max3(glm::vec3 a, aiVector3D b) {
  float x = a.x > b.x ? a.x : b.x;
  float y = a.y > b.y ? a.y : b.y;
  float z = a.z > b.z ? a.z : b.z;
  return glm::vec3(x, y, z);
}

glm::vec3 min3(glm::vec3 a, aiVector3D b) {
  float x = a.x < b.x ? a.x : b.x;
  float y = a.y < b.y ? a.y : b.y;
  float z = a.z < b.z ? a.z : b.z;
  return glm::vec3(x, y, z);
}

void Mesh::import(Scene* scene, const aiMesh* mesh) {
  struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;
    glm::vec2 tex;
  };

  vector<glm::uvec3> indices;
  vector<Vertex> vertices;

  material = scene->materials[mesh->mMaterialIndex];
  vsize = mesh->mNumVertices;
  isize = mesh->mNumFaces * 3;

  indices.resize(mesh->mNumFaces);
  for (uint j = 0; j < mesh->mNumFaces; ++j) {
    memcpy(&indices[j], mesh->mFaces[j].mIndices, sizeof(glm::uvec3));
  }
  if (mesh->HasPositions()) {
    memcpy(&bbMin, &mesh->mVertices[0], sizeof(glm::vec3));
    memcpy(&bbMax, &mesh->mVertices[0], sizeof(glm::vec3));
  }

  vertices.resize(mesh->mNumVertices);
  for (uint j = 0; j < mesh->mNumVertices; ++j) {
    if (mesh->HasPositions()) {
      bbMin = min3(bbMin, mesh->mVertices[j]);
      bbMax = max3(bbMax, mesh->mVertices[j]);
      memcpy(&vertices[j].pos, mesh->mVertices + j, sizeof(glm::vec3));
    }
    if (mesh->HasNormals()) {
      memcpy(&vertices[j].normal, mesh->mNormals + j, sizeof(glm::vec3));
    }
    if (mesh->HasTextureCoords(0)) {
      memcpy(&vertices[j].tex, mesh->mTextureCoords[0] + j, sizeof(glm::vec2));
    }
    if (!mesh->HasTangentsAndBitangents())
      continue;

    memcpy(&vertices[j].tangent, mesh->mTangents + j, sizeof(glm::vec3));
    memcpy(&vertices[j].bitangent, mesh->mBitangents + j, sizeof(glm::vec3));
  }

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, isize * 4, indices.data(),
               GL_STATIC_DRAW);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vsize * sizeof(Vertex), vertices.data(),
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, 0, 56, (void*)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, 0, 56, (void*)12);

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, 0, 56, (void*)24);

  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 3, GL_FLOAT, 0, 56, (void*)36);

  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 2, GL_FLOAT, 0, 56, (void*)48);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  if (mesh->HasBones()) {
    struct VertexBoneData {
      uint id[4];
      float wgt[4];
      VertexBoneData() : id{}, wgt{} {}
      void set(uint i, float w) {
        for (int j = 0; j < 4; ++j) {
          if (wgt[j] == 0) {
            wgt[j] = w;
            id[j] = i;
            break;
          }
        }
      }
    };
    animated = 1;
    vector<VertexBoneData> bone_data(mesh->mNumVertices);

    for (uint i = 0; i < mesh->mNumBones; ++i) {
      auto bone = mesh->mBones[i];
      string bone_name = bone->mName.C_Str();
      uint bone_idx;

      auto existing = scene->bonemap.find(bone_name);
      if (existing != scene->bonemap.end()) {
        bone_idx = existing->second.id;
      } else {
        bone_idx = scene->bonemap.size();
        scene->bonemap[bone_name].id = bone_idx;
        memcpy(&scene->bonemap[bone_name].offset, &bone->mOffsetMatrix, 64);
      }

      for (uint j = 0; j < bone->mNumWeights; ++j) {
        auto wgt = bone->mWeights[j];
        bone_data[wgt.mVertexId].set(bone_idx, wgt.mWeight);
      }
    }
    glBindVertexArray(vao);
    glGenBuffers(1, &skin_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, skin_buffer);
    glBufferData(GL_ARRAY_BUFFER, bone_data.size() * sizeof(VertexBoneData),
                 bone_data.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_UNSIGNED_INT, 32, 0);

    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, 0, 32, (void*)16);

    glBindVertexArray(0);
  }
}

void Mesh::render(RenderState& state) {
  state.active_shader->set_uniform("animated", animated);
  material->bind(state);
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, isize, GL_UNSIGNED_INT, 0);
}

Mesh::Mesh() : bbMin(), bbMax() {
  material = nullptr;
  animated = false;
  vao = 0;
  vbo = 0;
  ibo = 0;
  skin_buffer = 0;
  vsize = 0;
  isize = 0;
}

Mesh::~Mesh() {
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ibo);
  if (animated)
    glDeleteBuffers(1, &skin_buffer);
}
