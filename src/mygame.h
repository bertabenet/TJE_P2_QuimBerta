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

enum eIslandType{
    WOLVES,
    LIONS,
    PLAGUE,
    SHEPHERD,
    SHEEP_PARTY
};

enum eDirection{
    FORWARD,
    BACKWARDS,
    LEFT,
    RIGHT
};

enum eNPCType{
    CABBAGE,
    WOLF,
    SHEEP,
    NOONE
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
    bool visible;
    
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
    EntityMesh(Mesh* mesh, Texture* texture, Shader* shader, Vector4 color);

    Mesh* mesh;
    Texture* texture;
    Shader* shader;
    Vector4 color;

    //methods overwritten
    void render();
    //void update(float elapsed_time);
};

struct sNPC{
    Vector3 pos;
    eNPCType type;
    EntityMesh* mesh;
};

class Island
{
public:
    Vector3 pos;
    eIslandType type;
    EntityMesh* mesh;
};

class Level
{
public:
    int level;
    std::vector<Island*> islands;
};

class Player
{ // boat
public:
    eDirection dir;
    Island* current_island;
    EntityMesh* mesh;
    
    sNPC current_NPC;
    
    Player();
    Player(Vector3 init_pos, eDirection dir, Island* current_island, EntityMesh* mesh, sNPC current_NPC);
};

class World
{
public:
    static World* instance;
    std::vector<Entity*> entities;
    Player* boat;
    World();
    
    void addEntity(Entity* entity);
    void renderWorld();
};

#endif
