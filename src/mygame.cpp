#include "mygame.h"
#include "game.h"

void Entity::setName(std::string n) { name = n; };
void Entity::setModel(Matrix44 m) { model = m; };

EntityMesh::EntityMesh() {
    type = MESH;
    model.setIdentity();
}

EntityMesh::EntityMesh(Mesh* mesh, Texture* texture, Shader* shader, Vector4 color){//, Vector3 init_pos){
    this->mesh = mesh;
    this->texture = texture;
    this->shader = shader;
    this->color = color;
    
    type = MESH;
    model.setIdentity();
    //model.setTranslation(init_pos.x, init_pos.y, init_pos.z);
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
    for(int i=0; i<3;i++){
        //all_npc[i]->mesh->model.setTranslation(all_npc[i]->pos.x,all_npc[i]->pos.y,all_npc[i]->pos.z);
        all_npc[i]->mesh->render();
    }
    //render player
    //boat->mesh->model.setTranslation(boat->pos.x,boat->pos.y,boat->pos.z);
    boat->mesh->render();
}

Island::Island(Vector3 pos, eIslandType type, EntityMesh* mesh){
    this->pos = pos;
    this->type = type;
    this->mesh = mesh;
    mesh->model.setTranslation(pos.x,pos.y,pos.z);
    
    npc_vec = Vector3(0, 0, 0);
    for (int l=0; l<8; l++){links[l] = NULL;}
    //UNA POSSIBILITAT SERIA GUARDAR AQUÍ UN INT (QUE DE FET JA ES EL ENUM)
    //FENT REFERENCIA A UNA LLISTA DE Entities O MESHES DE ISLANDS (DO design jeje)
    //Una prova de com podria anar està a main
};

NPC::NPC(Vector3 pos, eNPCType type, EntityMesh* mesh){
    this->pos = pos;
    this->type = type;
    this->mesh = mesh;
    mesh->model.setTranslation(pos.x,pos.y,pos.z);
}

Player::Player(Vector3 init_pos, Island* current_island, EntityMesh* mesh){
    this->pos = init_pos;
    this->current_island = current_island;
    this->mesh = mesh;
    this->current_NPC = NULL;
    
    //this->mesh->model.translate(init_pos.x, init_pos.y, init_pos.z);
    Game::instance->world->boat = this;
}
int World::moveTo(Island* dest){
    Island* orig = boat->current_island;
    if (orig == NULL) return 0;
    else if (orig == dest) return 0;
    int ok = leave(orig);
    if (ok==0){
        std::cout<<"Left from: "<< orig->type <<std::endl;
        ok = arrive(dest);
        if (ok) std::cout<<"Problem arriving to: "<< dest->type <<std::endl;
        else std::cout<<"Arrived at: "<< dest->type <<std::endl;
    }
    else std::cout<<"Problem leaving from: "<< orig->type <<std::endl;
    return ok;
}

int World::leave(Island* island){
    boat->current_island = NULL;
    if (island->npc_vec[0] and island->npc_vec[1]){return 1;} //WOLF EATS SHEEP
    else if (island->npc_vec[1] and island->npc_vec[2]){return 1;} //SHEEP EATS CABBAGE
    else return 0;
}

int World::arrive(Island* island){
    boat->current_island = island;
    //boat->mesh->color = boat->current_island->mesh.color;
    if (island->type == WOLVES && boat->current_NPC->type==SHEEP){return 1;}
    else if (island->type == LIONS && (boat->current_NPC->type==WOLF ||
                                       boat->current_NPC->type==SHEEP)){return 1;}
    else if (island->type == PLAGUE && boat->current_NPC->type==CABBAGE){return 1;}
    else if (island->type == SHEPHERD && boat->current_NPC->type==WOLF){return 1;}
    else if (island->type == SHEEP_PARTY && (boat->current_NPC->type==WOLF ||
                                             boat->current_NPC->type==CABBAGE)){return 1;}
    else return 0;
}

void World::drop(){
    NPC* n = boat->current_NPC; //std::cout<<"a"<<std::endl;
    Island* i = boat->current_island; //std::cout<<"b"<<std::endl;
    i->addNPC(n); //std::cout<<"c"<<std::endl;
    //n->mesh->model.setTranslation(new_pos.x,new_pos.y,new_pos.z);
    boat->current_NPC = NULL; //std::cout<<"f"<<std::endl;
}

void World::pickup(NPC* npc){
    Island* i = boat->current_island;
    i->removeNPC(npc);
    Vector3 new_pos = 
        Vector3(boat->pos.x+10,
                boat->pos.y,
                boat->pos.z);
    npc->pos = new_pos;
    //npc->pmesh->model.setTranslation(new_pos.x,new_pos.y,new_pos.z);
    boat->current_NPC = npc;
}