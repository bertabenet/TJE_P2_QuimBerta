#ifndef MYGAME_H
#define MYGAME_H

#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"

enum eStage_types: uint8{
	//INTRO_STAGE, 
	MENU_STAGE,
    TUTORIAL_STAGE,
    PLAY_STAGE,
    PAUSE_STAGE,
    END_STAGE
};

enum eEntityType{
    MESH,
    OTHER
};

enum eIslandType{
    NORMAL,
    NORMAL2, //TODO: REMOVE, this is for testing 19/05/2021
    WOLVES,
    LIONS,
    PLAGUE,
    SHEPHERD,
    SHEEP_PARTY
};

enum eDirection{
    NORTH,
    EAST,
    WEST,
    SOUTH,
    NORTHEAST,
    NORTHWEST,
    SOUTHEAST,
    SOUTHWEST
};

enum eNPCType{
    WOLF,
    SHEEP,
    CABBAGE,
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
    EntityMesh(Mesh* mesh, Texture* texture, Shader* shader, Vector4 color);//, Vector3 init_pos);

    Mesh* mesh;
    Texture* texture;
    Shader* shader;
    Vector4 color;

    //methods overwritten
    void render();
    //void update(float elapsed_time);
};

class NPC{
public:
    Vector3 pos;
    eNPCType type;
    EntityMesh* mesh;
    
    NPC(Vector3 pos, eNPCType type, EntityMesh* mesh);
    void updatePos();
};

class Island
{
public:
    Vector3 pos;
    eIslandType type;
    EntityMesh* mesh;
    Vector3 npc_vec; //{W,S,C}
    Island* links[8];
    
    Island(Vector3 pos, eIslandType type, EntityMesh* mesh);
    void addNPC(NPC* npc){
        npc_vec.v[npc->type] = 1;
        npc->pos = Vector3(pos.x+npc->type*5,pos.y+10,pos.z);
    }
    void removeNPC(NPC* npc){npc_vec.v[npc->type] = 0;}
};

class Level
{
public:
    int level;
    std::vector<Island*> islands;
    void addIsland(Island* island){islands.push_back(island);}
};

class Player
{ // boat
public:
    //eDirection dir; // NECESSARI???
    Vector3 pos;
    Island* current_island;
    EntityMesh* mesh;
    
    NPC* current_NPC;
    int movesAlone = 0;
    
    Player();
    Player(Vector3 init_pos, Island* current_island, EntityMesh* mesh);
};

class World
{
public:
    static World* instance;
    std::vector<Entity*> entities;
    std::vector<NPC*> all_npc;
    std::vector<Island*> *islands;
    Player* boat;
    World(){};
    
    void addEntity(Entity* entity){entities.push_back(entity);}
    void renderWorld();
    int moveTo(Island* dest);
    int leave(Island* island);
    int arrive(Island* island);
    void drop();
    void pickup(NPC* npc);
};
#endif
