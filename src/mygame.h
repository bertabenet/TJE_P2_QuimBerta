#ifndef MYGAME_H
#define MYGAME_H

#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"

enum eEntityType{
    MESH,
    OTHER
};

class Entity
{
    public:
        //Entity(); //constructor
        //Entity(std::string n, Matrix44 * m);
        //virtual ~Entity(); //destructor

        //some attributes
        eEntityType type;
        std::string name;
        Matrix44 model;
        //methods overwritten by derived classes
        virtual void render(){};
        virtual void update(float elapsed_time){};

        //some useful methods...
        Vector3 getPosition();

        void setName(std::string n);
        void setModel(Matrix44 m);
};

class EntityMesh : public Entity
{
    public:
        //Attributes of this class
        EntityMesh();
        EntityMesh(std::string n, Matrix44 m);

        Mesh* mesh;
        Texture* texture;
        Shader* shader;
        Vector4 color;

        //methods overwritten
        void render();
        //void update(float elapsed_time);
};

class World
{
    public:
        static World* instance;
        World();
        std::vector<Entity*> e;
        void addEntity(Entity* entity);
        void renderWorld();
};

#endif
