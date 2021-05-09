#include "mygame.h"
#include "game.h"

void Entity::setName(std::string n) { name = n; };
void Entity::setModel(Matrix44 m) { model = m; };

EntityMesh::EntityMesh() {type = MESH;}

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

World::World() { }; //World::instance = this; }
void World::addEntity(Entity* entity) {
    e.push_back(entity);
}

void World::renderWorld(){
    //render entities tipus 1;
    for (int i = 0; i < e.size(); i++) {
        Matrix44 m;
        //m.rotate(angle*DEG2RAD, Vector3(0, 0, 1));
        Entity* current = e[i];
        current->setModel(m);
        current->render();
    }
    //render entities tipus 2:
    //blabla
}
