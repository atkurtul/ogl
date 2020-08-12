#pragma once
#include "material.h"

struct Mesh {

	uint vao, vbo, ibo, skin_buffer;

	uint vsize;
	uint isize;

	const Material* material;

	bool animated;

	glm::vec3 bbMin;
	glm::vec3 bbMax;

	void import(struct Scene* scene, const struct aiMesh* mesh);

	void render(RenderState&);

	Mesh();

	~Mesh();
};
