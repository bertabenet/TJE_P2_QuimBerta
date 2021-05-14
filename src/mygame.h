#ifndef MYGAME_H
#define MYGAME_H

#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"

enum stage_types: uint8{
	//INTRO_STAGE, 
	MENU_STAGE, TUTORIAL_STAGE, PLAY_STAGE, PAUSE_STAGE, END_STAGE
};

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
    Island(Vector3 pos, eIslandType type, EntityMesh* mesh);
    Vector3 pos;
    eIslandType type;
    EntityMesh* mesh;
    //FALTA ALGUNA COSA PER TRACKEJAR NPCs A LA ILLA
    //UN STD VECTOR? POTSER MÉS FACILÓN ROLLO UN ARRAY [0,0,1] O BITMAP (només n'hi ha tres)
};

class Level
{
public:
    int level;
    std::vector<Island*> islands;
    void addIsland(Island* island){islands.push_back(island);}
    //FALTA ALGUNA COSA PER TRACKEJAR QUINES ILLES PODEN MOURE'S A QUINES ALTRES ILLES
};

class Player
{ // boat
public:
    eDirection dir; // NECESSARI???
    Island* current_island;
    EntityMesh* mesh;
    
    sNPC current_NPC;
    int movesAlone = 0;
    
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
    
    void addEntity(Entity* entity){entities.push_back(entity);}
    void renderWorld();
};

class Stage {
	public:
		virtual void Render() = 0;
		virtual void Update(float elapsed_time) = 0;
};

class MenuStage : public Stage{
	public: 
		MenuStage(void){
			//this->world = new World(160,120,&(game->synth),Vector2(25, 30),5,5);
		};
		//World* world; 
		int option = 0;
		virtual void Render(); virtual void Update(float elapsed_time);};

class TutorialStage : public Stage{
	public: 
		TutorialStage(void){
			//this->world = new World(160,120,&(game->synth),Vector2(80,60),100,50);
		}
		//World* world; 
		int tutorial_stage = 0;
		virtual void Render(); virtual void Update(float elapsed_time);};

class PlayStage : public Stage{
	public: 
		/*PlayStage(TileMap* map, int n_s, int n_w){
			this->world = new World(160,120, &(game->synth), map, n_s, n_w);
		}*/
		World* world;
        std::vector<Level*> levels;
        void addLevel(Level* level){levels.push_back(level);}
		//int init_time;
		virtual void Render(); 
		virtual void Update(float elapsed_time);
};

class PauseStage : public Stage{
	public: PauseStage(void){}
		virtual void Render(); virtual void Update(float elapsed_time);};

class EndStage : public Stage{
	public: EndStage(void){}
		int stats[4];
		virtual void Render(); virtual void Update(float elapsed_time);};

#endif
