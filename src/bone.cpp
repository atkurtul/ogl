#include "bone.h"
#include "scene.h"
#include "assimp/scene.h"

void Bone::import(Scene* scene, const aiNode* ainode)
{
	name	= ainode->mName.C_Str();
	id		= scene->bonemap[name].id;
	offset	= glm::transpose(scene->bonemap[name].offset);
	memcpy(&transform, &ainode->mTransformation, 64);
	transform = glm::transpose(transform);

	for (uint i = 0; i < ainode->mNumMeshes; ++i) {
		meshes.push_back(scene->meshes[ainode->mMeshes[i]]);
	}

	for (unsigned int i = 0; i < ainode->mNumChildren; ++i) {
		if (scene->bonemap.find(ainode->mChildren[i]->mName.C_Str()) == scene->bonemap.end()) {
			continue;
		}
		Bone* child = new Bone;
		child->import(scene, ainode->mChildren[i]);
		children.push_back(child);
	}
}

void Bone::update(Scene* scene, glm::mat4 parent, glm::mat4* uniform)
{
	auto final = parent * scene->active_animation->get_frame(this);
	uniform[id] = final * offset;

	for (auto child : children) {
		child->update(scene, final, uniform);
	}
}

