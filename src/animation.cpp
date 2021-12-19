#include "animation.h"
#include "assimp/scene.h"
#include "scene.h"


void Animation::import(Scene* scene, const aiAnimation* anim) {
  duration = anim->mDuration;
  timer = 0;
  for (uint i = 0; i < anim->mNumChannels; ++i) {
    auto channel = anim->mChannels[i];
    auto& frame = keyframes[channel->mNodeName.C_Str()];

    for (uint j = 0; j < channel->mNumPositionKeys; ++j) {
      glm::vec3 pos;
      float time = channel->mPositionKeys[j].mTime;
      memcpy(&pos, &channel->mPositionKeys[j].mValue, sizeof(pos));
      frame.pos.push_back({pos, time});
    }

    for (int j = 0; j < channel->mNumRotationKeys; ++j) {
      glm::quat rot;
      float time = channel->mRotationKeys[j].mTime;
      auto q = channel->mRotationKeys[j].mValue;
      frame.rot.push_back({glm::quat(q.w, q.x, q.y, q.z), time});
    }

    for (int j = 0; j < channel->mNumScalingKeys; ++j) {
      glm::vec3 sca;
      float time = channel->mScalingKeys[j].mTime;
      memcpy(&sca, &channel->mScalingKeys[j].mValue, sizeof(sca));
      frame.sca.push_back({sca, time});
    }
  }
}

glm::mat4 Animation::get_frame(Bone* bone) {
  glm::mat4 re = bone->transform;
  auto pair = keyframes.find(bone->name);
  if (pair == keyframes.end())
    return re;

  auto& frame = pair->second;
  frame.get_matrix(timer, re);
  return re;
}

void Animation::update(float dt) {
  timer = fmod(timer + dt, duration);
}
