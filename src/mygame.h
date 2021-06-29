#ifndef MYGAME_H
#define MYGAME_H

#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"

enum eStage_types: uint8{
	//INTRO_STAGE, 
	MENU_STAGE,
    //TUTORIAL_STAGE,
    PLAY_STAGE,
    //PAUSE_STAGE,
    END_STAGE,
    CREDITS_STAGE
};

enum eEntityType{
    MESH,
    OTHER
};

enum eIslandType{
    i_EMPTY,
    i_ISLAND,
    i_EXTREME,
    i_PLAGUE,
    i_WOLVES,
    i_LIONS,
    i_SHEPHERD,
    i_SHEEP_PARTY
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
    float t_coef = 0.0f; //TODO: aqui?
    //float tiling = 1.0f; //TODO: aqui?

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
    Vector2 tilemap_pos;
    int index_inVector;
    bool endgame = false;
    Island* links[8];
    
    Island(Vector3 pos, eIslandType type, EntityMesh* mesh);
    void addNPC(NPC* npc){
        npc_vec.v[npc->type] = 1;
        npc->pos = Vector3(pos.x+npc->type*7-7,pos.y,pos.z-10);
        if(npc->type==CABBAGE) npc->pos.y+=5;
    }
    void removeNPC(NPC* npc){npc_vec.v[npc->type] = 0;}
};

/*class Level
{
public:
    int level;
    std::vector<Island*> islands;
    void addIsland(Island* island){islands.push_back(island);}
};*/

class Player
{ // boat
public:
    //eDirection dir; // NECESSARI???
    Vector3 pos;
    Vector2 moving = Vector2(0,0);
    //Vector3 circling;

    //float time_spent = 0;
    //Vector3 pos_arrival = Vector3(0,0,0);
    //float angle_test = 0;
    float angle_arrival = 0;
    int mov_ind = 8;
    Island* current_island;
    Island* previous_island;
    float hurt = 0; 
    EntityMesh* mesh;
    
    NPC* current_NPC;
    int movesAlone = 0;
    int total_moves = 0;
    int lives = 3;
    
    Player();
    Player(Vector3 init_pos, Island* current_island, EntityMesh* mesh);
};

enum eCellType : uint8 { 
    c_EMPTY,
    c_ISLAND,
    c_EXTREME,
    c_PLAGUE,
    c_WOLVES,
    c_LIONS,
    c_SHEPHERD,
    c_SHEEP_PARTY,
    c_WATER1,
    c_WATER2,
    c_WATER3,
    c_WATER4,
    c_WATER5,
    c_WATER6,
    c_WATER7,
    c_WATER8,
};

struct sCell {
    eCellType type;   
};

class TileMap {
public:
    int width;
    int height;
    sCell* data;

    TileMap()
    {
        width = height = 0;
		data = NULL;
    }

    TileMap(int w, int h)
    {
        width = w;
        height = h;
        data = new sCell[w*h];
    }

    sCell& getCell(int x, int y)
    {
        return data[x + y * width];
    }

	sCell& getCell(Vector2 pos)
    {
        return getCell(pos.x, pos.y);
    }

	sCell& getCellFromWorldPos(Vector2 pos)
    {
        return getCell((int)pos.x/12, (int)pos.y/12);
    }
    void printMap(){
        for (int w = 0; w<width; w++)
            for (int h = 0; h<height; h++){
                std::cout<<char(int(data[w+h*width].type))<<std::endl;
            }
    }
};

struct sMapHeader {
    int w; //width of map
    int h; //height of map
    unsigned char bytes; //num bytes per cell
    unsigned char extra[7]; //filling bytes, not used
};

TileMap* loadGameMap(const char* filename);

class World
{
public:
    static World* instance;
    TileMap* gamemap;
    EntityMesh* sky;
    EntityMesh* sea;
    EntityMesh* seapath;
    
    // WATER
    Texture* water_detail_texture; // normalmap of water
    EntityMesh* water;
    
    std::vector<Entity*> entities;
    std::vector<NPC*> all_npc;
    const char* npc_sounds[3] = {"data/assets/Sound/woof.wav","data/assets/Sound/bleat.wav","data/assets/Sound/wet.wav"};
    std::vector<Island*> islands;
    Player* boat;
    float tile_offset=10.0f; //TODO: Guarrada? moure?
    bool moving_track = true;
    bool birdview = false;
    
    bool closeview = false;
    int close_focus = 0;
    World();
    
    void addEntity(Entity* entity){entities.push_back(entity);}
    void renderWorld();
    int moveTo(Island* dest);
    int leave(Island* island);
    int arrive(Island* island);
    void drop();
    void pickup(NPC* npc);
    void renderWater();

    void setup_level(TileMap* map);
    void setup_links();

    int check_end();
};

#endif

