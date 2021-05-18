#ifndef STAGEMANAGER_H
#define STAGEMANAGER_H

#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "mygame.h"

class Stage {
public:
    virtual void render() = 0;
    virtual void update(float elapsed_time) = 0;
};


class MenuStage : public Stage{
public:
    MenuStage(void){
        //this->world = new World(160,120,&(game->synth),Vector2(25, 30),5,5);
    };
    //World* world;
    int option = 0;
    virtual void render();
    virtual void update(float elapsed_time);
    
};


class TutorialStage : public Stage{
public:
    TutorialStage(void){
        //this->world = new World(160,120,&(game->synth),Vector2(80,60),100,50);
    }
    //World* world;
    int tutorial_stage = 0;
    virtual void render();
    virtual void update(float elapsed_time);};


class PlayStage : public Stage{
public:
    int current_level;
    std::vector<Level*> levels;
    void addLevel(Level* level){levels.push_back(level);}
    
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
