#pragma once
#include "mesh.h"
#include "camera.h"
#include "light.h"
#include "animation.h"
#include "bone.h"
#include "physics.h"

struct Scene {
	vector<Material*>	materials;
	vector<Mesh*>		meshes;
	vector<Light*>		lights;
	vector<Camera*>		cameras;
	vector<Animation*>	animations;
	unordered_map<string, Texture*>	textures;

	PhysicsSimulator simulator;

	Node*			root;
	Camera*			active_cam;
	Material*		default_mat;
	Animation*	    active_animation;
	Shader			default_shader;
	Shader			shadow_shader;

	uint fbo, depth_buffer;
	Window win;

	struct BoneData {
		glm::mat4 offset;
		uint id;
	};

	unordered_map<string, BoneData> bonemap;

	Scene();

	~Scene();

	void update(float dt);

	Texture* get_texture(string name, Texture::Slot slot);

	Scene& import(string path);

	void render();

	void physics_demo();

};