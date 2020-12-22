#include "pbr.h"

void Sandbox::place_ent(Engine* eng)
{
    Entity*  box = new Entity();
    Model* mod = new Model();
    Model* lightMod = new Model();
    glm::vec3 lightPositions[] = {
        glm::vec3(-3.0f,  1.0f, 3.0f),
        glm::vec3(3.0f,  1.0f, 3.0f),
        glm::vec3(-3.0f, -1.0f, 3.0f),
        glm::vec3(3.0f, -1.0f, 3.0f),
    };
    glm::vec3 lightColors[] = {
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f)
    };
    mod->load_obj("res/models/test.obj", true);
    //mod->set_shader("res/shaders/pbr_vert.glsl", "res/shaders/pbr_frag.glsl");
    mod->set_shader("res/shaders/stand_vertex.glsl", "res/shaders/stand_fragment.glsl");
    mod->load_texture("res/textures/wall.jpg");

    lightMod->load_obj("res/models/test.obj", true);
    lightMod->set_shader("res/shaders/light_vertex.glsl", "res/shaders/light_fragment.glsl");

    box->set_model(mod);
    eng->add_entity(box);
    box->move_to(0.0f, 0.0f, 0.0f);
    box->rotate(0.0f, 30.0f, 0.0f);

    Entity* light = new Entity();
    light->set_model(lightMod);
    eng->add_entity(light);
    light->move_to(lightPositions[0].x, lightPositions[0].y, lightPositions[0].z);
    light->scale(0.5f, 0.5f, 0.5f);
    eng->add_light_source(lightPositions[0], lightColors[0]);
}