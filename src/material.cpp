#include "scene.h"
#include "assimp/scene.h"

void Material::import(struct Scene* scene, const aiMaterial* mat)
{
	aiColor3D diffuse(1.f);
	aiColor3D specular(.5);
	aiColor3D ambient(.2f);
	aiColor3D emissive(0.f);
	mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
	mat->Get(AI_MATKEY_COLOR_SPECULAR, specular);
	mat->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
	mat->Get(AI_MATKEY_COLOR_EMISSIVE, emissive);
	memcpy(&diffuse, &diffuse, sizeof(aiColor3D));
	memcpy(&specular, &specular, sizeof(aiColor3D));
	memcpy(&ambient, &ambient, sizeof(aiColor3D));
	memcpy(&emissive, &emissive, sizeof(aiColor3D));

	if (mat->GetTextureCount(aiTextureType_DIFFUSE)) {
		aiString str;
		mat->GetTexture(aiTextureType_DIFFUSE, 0, &str);
		textures[Texture::DIFFUSE] = scene->get_texture(str.C_Str(), Texture::DIFFUSE);
	}
	else {
		textures[Texture::DIFFUSE] = scene->default_mat->textures[Texture::DIFFUSE];
	}

	if (mat->GetTextureCount(aiTextureType_HEIGHT)) {
		aiString str;
		mat->GetTexture(aiTextureType_HEIGHT, 0, &str);
		textures[Texture::NORMAL] = scene->get_texture(str.C_Str(), Texture::NORMAL);
	}
	else {
		textures[Texture::NORMAL] = scene->default_mat->textures[Texture::NORMAL];
	}
}

void Material::bind(RenderState& state) const
{
	state.active_shader->set_uniform("diffuse", diffuse);
	state.active_shader->set_uniform("specular", specular);
	state.active_shader->set_uniform("ambient", ambient);
	state.active_shader->set_uniform("emissive", emissive);
	
	textures[Texture::DIFFUSE]->bind(Texture::DIFFUSE);
	textures[Texture::NORMAL]->bind(Texture::NORMAL);
}
