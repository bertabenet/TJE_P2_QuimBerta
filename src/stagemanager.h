#ifndef STAGEMANAGER_H
#define STAGEMANAGER_H

#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "mygame.h"
#include "camera.h"

enum eMenuButton{
    PLAY_BUTTON,
    QUIT_BUTTON
};

class Stage {
public:
    virtual void render() = 0;
    virtual void update(float elapsed_time) = 0;
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
    EntityMesh* penguin;
    EntityMesh* bear;
    EntityMesh* rat;
    std::vector<EntityMesh*> flowers;

    float acumulated_rotation = 0.0;
    
    eMenuButton selected;

    virtual void render();
    virtual void update(float elapsed_time);
    
    void menuMaker(EntityMesh* to_move);
    
};


class TutorialStage : public Stage{
public:
    TutorialStage(void);
    //World* world;
    int tutorial_stage = 0;
    virtual void render();
    virtual void update(float elapsed_time);};


class PlayStage : public Stage{
public:
    PlayStage(void);
    World* world;
    int current_level = 0;
    std::vector<TileMap*> levels;
    void addLevel(TileMap* level){levels.push_back(level);}
    
    virtual void render();
    virtual void update(float elapsed_time);
};


class PauseStage : public Stage{
public:
    PauseStage(void){}
    virtual void render();
    virtual void update(float elapsed_time);
    
};


class EndStage : public Stage{
public:
    EndStage(void){}
    int stats[4];
    virtual void render();
    virtual void update(float elapsed_time);
    
};

#endif
