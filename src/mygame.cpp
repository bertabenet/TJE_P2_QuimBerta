#include "mygame.h"
#include "game.h"

void Entity::setName(std::string n) { name = n; };
void Entity::setModel(Matrix44 m) { model = m; };

EntityMesh::EntityMesh() {
    type = MESH;
    model.setIdentity();
}

EntityMesh::EntityMesh(Mesh* mesh, Texture* texture, Shader* shader, Vector4 color, Vector3 init_pos){
    this->mesh = mesh;
    this->texture = texture;
    this->shader = shader;
    this->color = color;
    
    type = MESH;
    model.setIdentity();
    model.setTranslation(init_pos.x, init_pos.y, init_pos.z);
}

void EntityMesh::render()
{
    //get the last camera that was activated
    Camera* camera = Camera::current;
    
    //enable shader and pass uniforms
    shader->enable();
    shader->setUniform("u_color", color);
    shader->setUniform("u_model", model);
    shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
    shader->setTexture("u_texture", texture, 0);
    //shader->setUniform("u_time", time);

    //render the mesh using the shader
    mesh->render(GL_TRIANGLES);

    //disable the shader after finishing rendering
    shader->disable();
}

void World::renderWorld(){
    //render islands
    std::vector<Island*> I_vector = *islands;
    for (int i = 0; i < I_vector.size(); i++) {
        Island* I_current = I_vector[i];
        Entity* E_current = I_current->mesh;
        E_current->render();
    }
    //for(int i=0; i<3;i++){
    //    all_npc[i]->mesh->render();
        all_npc[0]->mesh->render();
    //}
    //render player
    boat->mesh->render();
}

Island::Island(Vector3 pos, eIslandType type, EntityMesh* mesh){
    this->pos = pos;
    this->type = type;
    this->mesh = mesh;
    
    npc_vec = Vector3(0, 0, 0);
    //UNA POSSIBILITAT SERIA GUARDAR AQUÍ UN INT (QUE DE FET JA ES EL ENUM)
    //FENT REFERENCIA A UNA LLISTA DE Entities O MESHES DE ISLANDS (DO design jeje)
    //Una prova de com podria anar està a main
};

NPC::NPC(Vector3 pos, eNPCType type, EntityMesh* mesh){
    this->pos = pos;
    this->type = type;
    this->mesh = mesh;
}

Player::Player(Vector3 init_pos, eDirection dir, Island* current_island, EntityMesh* mesh, NPC* current_NPC){
    this->dir = dir;
    this->current_island = current_island;
    this->mesh = mesh;
    this->current_NPC = current_NPC;
    
    this->mesh->model.translate(init_pos.x, init_pos.y, init_pos.z);
    Game::instance->world->boat = this;
}
