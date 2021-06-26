#ifndef STAGEMANAGER_H
#define STAGEMANAGER_H

#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "mygame.h"
#include "camera.h"

enum eButton{
    PLAY_BUTTON,
    QUIT_BUTTON
};

class Stage {
public:
    virtual void render() = 0;
    virtual void update(float elapsed_time) = 0;
    
    float acumulated_rotation = 0.0;
    void sceneMaker(EntityMesh* to_move, float speed);
};


class MenuStage : public Stage{
public:
    MenuStage(void);
    //World* world;
    int option = 0;
    EntityMesh* play_button;
    EntityMesh* quit_button;
    EntityMesh* island;
    EntityMesh* boat;
    std::vector<EntityMesh*> flowers;
        
    eButton selected;

    virtual void render();
    virtual void update(float elapsed_time);
    
};


/*class TutorialStage : public Stage{
public:
    TutorialStage(void);
    //World* world;
    int tutorial_stage = 0;
    virtual void render();
    virtual void update(float elapsed_time);};
*/

class PlayStage : public Stage{
public:
    PlayStage(void);
    World* world;
    int current_level = 0;
    std::vector<TileMap*> levels;
    
    bool show_instructions = true;
    EntityMesh* instructions_quad;
    
    void addLevel(TileMap* level){levels.push_back(level);}
    
    virtual void render();
    virtual void update(float elapsed_time);
};


/*class PauseStage : public Stage{
public:
    PauseStage(void){}
    virtual void render();
    virtual void update(float elapsed_time);
    
};*/


class EndStage : public Stage{
public:
    EndStage(void);
    int winlose;
    int stats[4];
    EntityMesh* island;
    EntityMesh* boat;
    EntityMesh* text;
    EntityMesh* quad;
    
    EntityMesh* play_button;
    EntityMesh* quit_button;
    eButton selected;
    
    bool sink;
    std::vector<EntityMesh*> lostFlowers;
    std::vector<EntityMesh*> winFlowers;
        
    virtual void render();
    virtual void update(float elapsed_time);
    
    void updateLost();
    void updateWon();
    void pushLostFlowers(Mesh* m1, Mesh* m2, Texture* tex, Shader* s);
    void pushWinFlowers(Mesh* m1, Mesh* m2, Mesh* m3, Mesh* m4, Mesh* m5, Mesh* m6, Mesh* m7, Mesh* m8, Mesh* m9, Texture* tex, Shader* s);
    
};

class CreditsStage : public Stage{
public:
    CreditsStage(void);
    
    EntityMesh* island;
    EntityMesh* boat;
    EntityMesh* text;
    EntityMesh* quad;
    
    EntityMesh* play_button; //menu?
    EntityMesh* quit_button;
    eButton selected;
    
    bool sink;
    std::vector<EntityMesh*> lostFlowers;
    std::vector<EntityMesh*> winFlowers;
        
    virtual void render();
    virtual void update(float elapsed_time);
    
    //void updateLost();
    //void updateWon();
    //void pushLostFlowers(Mesh* m1, Mesh* m2, Texture* tex, Shader* s);
    //void pushWinFlowers(Mesh* m1, Mesh* m2, Mesh* m3, Mesh* m4, Mesh* m5, Mesh* m6, Mesh* m7, Mesh* m8, Mesh* m9, Texture* tex, Shader* s);
    
};

#endif
