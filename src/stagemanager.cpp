#include "stagemanager.h"
#include "game.h"
#include "input.h"

/*void PlayStage::render()
{
    //Level* l = levels[current_level];
    //l->renderIslands();
}*/

void PlayStage::render(){
    Game::instance->world->renderWorld(); //risky? no guardem referencia del mon al stage (perq no cal)
    //levels[Game::instance->curr_playstage]->islands->render();
}

void PlayStage::update(float seconds_elapsed){
    float speed = seconds_elapsed * Game::instance->mouse_speed; //the speed is defined by the seconds_elapsed so it goes constant

    Camera* camera = Game::instance->camera;
    bool* mouse_locked = &(Game::instance->mouse_locked);

    //mouse input to rotate the cam
    if ((Input::mouse_state & SDL_BUTTON_LEFT) || *mouse_locked ) //is left button pressed?
    {
        camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f,-1.0f,0.0f));
        camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector( Vector3(-1.0f,0.0f,0.0f)));
    }

    Player* player = Game::instance->world->boat;
    //async input to move the camera around
    if(Input::isKeyPressed(SDL_SCANCODE_LSHIFT) ) speed *= 10; //move faster with left shift
    if (Input::isKeyPressed(SDL_SCANCODE_W)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
    if (Input::isKeyPressed(SDL_SCANCODE_S)) camera->move(Vector3(0.0f, 0.0f,-1.0f) * speed);
    if (Input::isKeyPressed(SDL_SCANCODE_A)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
    if (Input::isKeyPressed(SDL_SCANCODE_D)) camera->move(Vector3(-1.0f,0.0f, 0.0f) * speed);
    
    /*
    if (Input::isKeyPressed(SDL_SCANCODE_UP)) player->mesh->model.translate(0.0f, 0.0f, -0.1f);
    if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) player->mesh->model.translate(0.0f, 0.0f, 0.1f);
    if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) player->mesh->model.translate(-0.1f, 0.0f, 0.0f);
    if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) player->mesh->model.translate(0.1f,0.0f, 0.0f);
    */
    
    if (Input::isKeyPressed(SDL_SCANCODE_UP)) player->pos.z += -0.1f; //mesh->model.translate(0.0f, 0.0f, -0.1f);
    if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) player->pos.z += 0.1f;//player->mesh->model.translate(0.0f, 0.0f, 0.1f);
    if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) {
        player->pos.x += -1.0f;//player->mesh->model.translate(-0.1f, 0.0f, 0.0f);
        if(player->current_island->links[WEST]!=NULL){ //proof of concept of use of "links"
            int end = Game::instance->world->moveTo(player->current_island->links[WEST]);
            if (end) Game::instance->must_exit=true;
        }
    }
    if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) {
        player->pos.x += 1.0f;//player->mesh->model.translate(0.1f,0.0f, 0.0f);
        if(player->current_island->links[EAST]!=NULL){
            int end = Game::instance->world->moveTo(player->current_island->links[EAST]);
            if (end) Game::instance->must_exit=true;
        }
    }
    
    //TO PRINT INFO ABOUT CURRENT ISLAND AND NPC
        //if (player->current_NPC != NULL) std::cout<<"n:"<<player->current_NPC->type<<std::endl;
        //if (player->current_island != NULL) std::cout<<"i:"<<player->current_island->type<<std::endl;
    
    //ALTERNATIVE TO MOVE BETWEEN ISLANDS
        if (Input::wasKeyPressed(SDL_SCANCODE_1))  {
            int end = Game::instance->world->moveTo(levels[0]->islands[0]);
            if(end) Game::instance->must_exit=true;
            //std::cout<<"curr:0"<<std::endl;
        }
        if (Input::wasKeyPressed(SDL_SCANCODE_2)){
            int end = Game::instance->world->moveTo(levels[0]->islands[1]);
            if(end) Game::instance->must_exit=true;
            //std::cout<<"curr:1"<<std::endl;
        }
        //if (Input::wasKeyPressed(SDL_SCANCODE_3)){
        //    player->current_island = NULL;
        //    //std::cout<<"curr:NULL"<<std::endl;
        //}
    
    if (player->current_island != NULL && player->current_NPC == NULL){
        if (Input::wasKeyPressed(SDL_SCANCODE_7) && player->current_island->npc_vec[0]){
            Game::instance->world->pickup(Game::instance->world->all_npc[0]);
        }
        if (Input::wasKeyPressed(SDL_SCANCODE_8) && player->current_island->npc_vec[1]){
            Game::instance->world->pickup(Game::instance->world->all_npc[1]);
        }
        if (Input::wasKeyPressed(SDL_SCANCODE_9) && player->current_island->npc_vec[2]){
            Game::instance->world->pickup(Game::instance->world->all_npc[2]);
        }
    } 
    if (player->current_island != NULL && player->current_NPC != NULL && Input::wasKeyPressed(SDL_SCANCODE_0)){
        Game::instance->world->drop();
    }
    
    if (player->current_NPC != NULL){
        Vector3 new_pos = 
        Vector3(player->pos.x,
                player->pos.y,
                player->pos.z+1);
        player->current_NPC->pos = new_pos;
    }

    player->mesh->model.setTranslation(player->pos.x,player->pos.y,player->pos.z);
    //player->mesh->model.scale(2, 2, 2);
    std::vector<NPC*> all_npc = Game::instance->world->all_npc;
    for(int i=0; i<3;i++){
        //all_npc[i]->mesh->model.setScale(5, 5, 5);
        all_npc[i]->mesh->model.setTranslation(all_npc[i]->pos.x,all_npc[i]->pos.y,all_npc[i]->pos.z);
        all_npc[i]->mesh->model.scale(5, 5, 5);
    }

    //to navigate with the mouse fixed in the middle
    if (*mouse_locked)
        Input::centerMouse();

    if (Input::wasKeyPressed(SDL_SCANCODE_P)) Game::instance->curr_stage = PAUSE_STAGE;
}

void PauseStage::render(){
    //Game::instance->world->renderWorld(); //risky? no guardem referencia del mon al stage (perq no cal)
    //levels[Game::instance->curr_playstage]->islands->render();
}

void PauseStage::update(float seconds_elapsed){
    if (Input::wasKeyPressed(SDL_SCANCODE_P)) Game::instance->curr_stage = PLAY_STAGE;
}
