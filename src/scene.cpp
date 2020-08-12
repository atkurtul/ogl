#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "glad/glad.h"
#include "scene.h"

Scene &Scene::import(string path)
{
    printf("importing scene\n");
    Assimp::Importer imp;
    auto scene = imp.ReadFile(path,
                              aiProcess_Triangulate |
                                  aiProcess_FlipUVs |
                                  aiProcess_GenSmoothNormals |
                                  aiProcess_CalcTangentSpace |
                                  aiProcess_LimitBoneWeights);

    if (!scene)
        printf("assimp scene null\n");
    for (uint i = 0; i < scene->mNumMaterials; ++i)
    {
        Material *mat = new Material();
        mat->import(this, scene->mMaterials[i]);
        materials.push_back(mat);
    }

    printf("imported mats\n");
    for (uint i = 0; i < scene->mNumMeshes; ++i)
    {
        Mesh *mesh = new Mesh;
        mesh->import(this, scene->mMeshes[i]);
        meshes.push_back(mesh);
    }
    printf("imported meshes\n");
    for (uint i = 0; i < scene->mNumAnimations; ++i)
    {
        Animation *animation = new Animation;
        animation->import(this, scene->mAnimations[i]);
        animations.push_back(animation);
        active_animation = animation;
    }
    printf("imported anims\n");
    root->import(this, scene->mRootNode);
    printf("imported nodes\n");
    return *this;
}

void Scene::render()
{
    printf("rendering\n");
    Light light;
    light.create(0.1f, 120.f, 60.f, glm::vec3(-15.f, 50.f, -10.f), glm::vec3(5.f, 0.f, 5.f));
    while (win.poll())
    {
        update(win.dt);
        RenderState state;
        //render to depth buffer from the pov of light
        light.update_position(glm::angleAxis(win.dt * 0.2f, glm::vec3(0.f, 1.f, 0.f)) * light.position);
        {
            state.active_shader = &shadow_shader;
            glViewport(0, 0, 4096, 4096);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            glClear(GL_DEPTH_BUFFER_BIT);
            shadow_shader.bind();
            shadow_shader.set_uniform("xform", glm::mat4(1.f));
            shadow_shader.set_uniform("light_view", light.view);
            shadow_shader.set_uniform("light_prj", light.prj);
            root->render(state, glm::mat4(1.f));

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            int w, h;
            win.get_size(w, h);
            glViewport(0, 0, w, h);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, depth_buffer);
        }
        {
            state.active_shader = &default_shader;
            default_shader.bind();
            default_shader.set_uniform("xform", glm::mat4(1.f));
            default_shader.set_uniform("prj", active_cam->update(win));
            default_shader.set_uniform("light_view", light.view);
            default_shader.set_uniform("light_prj", light.prj);
            default_shader.set_uniform("light_pos", light.position);
            default_shader.set_uniform("eye_pos", active_cam->position);
            default_mat->bind(state);
            root->render(state, glm::mat4(1.f));
        }
    }
}

void Scene::physics_demo()
{
    string base = PROJECT_BASE_BUILD_DIR;
    import(base + "/cube/plane_cube.obj");
    for (auto node : root->children)
    {
        if (node->name == "Cube")
        {
            for (int i = -5; i < 5; i += 2)
            {
                for (int j = -5; j < 5; j += 2)
                {
                    for (int k = 10; k < 20; k += 2)
                    {
                        auto mesh = node->meshes[0];
                        auto body = simulator.add_body(mesh->bbMin, mesh->bbMax, glm::vec3(i, k, j), glm::vec3());
                        node->bodies[mesh].push_back(body);
                    }

                    for (int k = 50; k < 60; k += 2)
                    {
                        auto mesh = node->meshes[0];
                        auto body = simulator.add_body(mesh->bbMin, mesh->bbMax, glm::vec3(i, k, j), glm::vec3());
                        node->bodies[mesh].push_back(body);
                    }
                }
            }
        }
        if (node->name == "Plane")
        {
            auto mesh = node->meshes[0];
            auto body = simulator.add_body(mesh->bbMin, mesh->bbMax, glm::vec3(0, -1, 0), glm::vec3(0.f));
            node->bodies[mesh].push_back(body);
        }
    }

    render();
}

Scene::Scene() : win(1600, 900, "OGL")
{
    simulator.init();
    active_animation = 0;
    root = new Node;
    for (int i = 0; i < 10; ++i)
    {
        const float pi = 3.1415926535f * 2.f;
        const float angle = i * pi / 8.f;
        const float r = 15;
        cameras.push_back(new Camera(60.f, 16.f / 9.f, glm::vec3(cosf(angle) * r, 20, sinf(angle) * r), glm::vec3(0)));
    }
    active_cam = cameras[2];
    auto diffuse = new Texture;
    auto normal = new Texture;

    unsigned char diff[4] = {255, 255, 255, 255};
    unsigned char norm[4] = {0, 0, 127, 0};

    diffuse->from_raw(diff, 1, 1, GL_RGBA);
    normal->from_raw(norm, 1, 1, GL_RGBA);

    default_mat = new Material;
    default_mat->textures[Texture::DIFFUSE] = diffuse;
    default_mat->textures[Texture::NORMAL] = normal;
    string base = PROJECT_BASE_BUILD_DIR;
    default_shader.load(base + "/shaders/main.vert", base + "/shaders/main.frag");
    shadow_shader.load(base + "/shaders/shadow.vert", base + "/shaders/shadow.frag");

    glGenFramebuffers(1, &fbo);
    glGenTextures(1, &depth_buffer);
    glBindTexture(GL_TEXTURE_2D, depth_buffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 4096, 4096, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_buffer, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Scene::~Scene()
{
    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &depth_buffer);

    default_mat->textures[Texture::DIFFUSE]->free();
    default_mat->textures[Texture::NORMAL]->free();
    delete default_mat;
    delete root;

    for (auto t : textures)
        t.second->free();
    for (auto m : materials)
        delete m;
    for (auto m : meshes)
        delete m;
    for (auto c : cameras)
        delete c;
    for (auto a : animations)
        delete a;
}

void Scene::update(float dt)
{
    for (int i = 0; i < 10; ++i)
    {
        if (win.get_key(GLFW_KEY_0 + i))
        {
            active_cam = cameras[i];
            break;
        }
    }

    simulator.integrate(dt);

    if (active_animation)
    {
        glm::mat4 bones[128];
        active_animation->update(dt * 25);
        root->update(this, glm::mat4(1.f), bones);
        default_shader.bind();
        default_shader.set_uniform_array("bones", bones, sizeof(bones));
        shadow_shader.bind();
        shadow_shader.set_uniform_array("bones", bones, sizeof(bones));
    }
}

Texture *Scene::get_texture(string path, Texture::Slot slot)
{
    {
        auto tex = textures.find(path);
        if (tex != textures.end())
            return tex->second;
    }
    Texture *tex = new Texture{};
    if (tex->load(path))
    {
        textures[path] = tex;
        return tex;
    }
    delete tex;
    return default_mat->textures[slot];
}
