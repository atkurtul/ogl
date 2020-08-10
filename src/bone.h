#pragma once
#include "node.h"

struct Bone : Node {
	glm::mat4 offset;
	uint id;

	virtual void import(struct Scene* scene, const struct aiNode* ainode) override;
	virtual void update(Scene* scene, glm::mat4 parent, glm::mat4* uniform) override;
};