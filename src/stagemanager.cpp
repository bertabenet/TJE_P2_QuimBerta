#include "stagemanager.h"
#include "game.h"
#include "input.h"

MenuStage::MenuStage(void){
    Camera* camera = Game::instance->camera;
    camera->lookAt(Vector3(0.f, 5.f, 50.f),Vector3(0.f,0.f,20.f), Vector3(0.f,1.f,0.f)); 
    camera->setPerspective(70.f,Game::instance->window_width/(float)Game::instance->window_height,0.1f,10000.f); 
}

void MenuStage::render(){
    Camera* camera = Game::instance->camera;
    Game::instance->world->sky->model.setTranslation(camera->eye.x,camera->eye.y,camera->eye.z);
    Game::instance->world->sky->render();
    Game::instance->world->sea->render();
    Mesh* m = new Mesh();
    m->createQuad(0,0,30,10,true);
    EntityMesh* em = new EntityMesh(m, Texture::getWhiteTexture(), Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs"), Vector4(1,0,0,0));//, Vector3 init_pos);
    em->model.setTranslation(0,option*20,0.0);
    em->render();
}

void MenuStage::update(float seconds_elapsed){
    if (Input::wasKeyPressed(SDL_SCANCODE_UP) && option == 1) option = 0;
    if (Input::wasKeyPressed(SDL_SCANCODE_DOWN) && option == 0) option = 1;
    if (Input::wasKeyPressed(SDL_SCANCODE_RETURN) && option == 1) Game::instance->curr_stage = PLAY_STAGE;
}

PlayStage::PlayStage(void){
    world = Game::instance->world;
}

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

    if (Input::isKeyPressed(SDL_SCANCODE_M)) world->sea->tiling += 1.0;
    if (Input::isKeyPressed(SDL_SCANCODE_N)) world->sea->tiling -= 1.0;
    
    
    if (Input::isKeyPressed(SDL_SCANCODE_UP)) world->boat->pos.z += -0.1f; //mesh->model.translate(0.0f, 0.0f, -0.1f);
    if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) world->boat->pos.z += 0.1f;//player->mesh->model.translate(0.0f, 0.0f, 0.1f);
    if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) {
        world->boat->pos.x += -1.0f;//world->boat->mesh->model.translate(-0.1f, 0.0f, 0.0f);
        if(world->boat->current_island->links[WEST]!=NULL){ //proof of concept of use of "links"
            int end = world->moveTo(world->boat->current_island->links[WEST]);
            if (end) Game::instance->must_exit=true;
        }
    }
    if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) {
        world->boat->pos.x += 1.0f;//world->boat->mesh->model.translate(0.1f,0.0f, 0.0f);
        if(world->boat->current_island->links[EAST]!=NULL){
            int end = world->moveTo(world->boat->current_island->links[EAST]);
            if (end) Game::instance->must_exit=true;
        }
    }
    
    //TO PRINT INFO ABOUT CURRENT ISLAND AND NPC
        //if (world->boat->current_NPC != NULL) std::cout<<"n:"<<world->boat->current_NPC->type<<std::endl;
        //if (world->boat->current_island != NULL) std::cout<<"i:"<<world->boat->current_island->type<<std::endl;
    
    //ALTERNATIVE TO MOVE BETWEEN ISLANDS
        if (Input::wasKeyPressed(SDL_SCANCODE_1))  {
            int end = world->moveTo(levels[0]->islands[0]);
            if(end) Game::instance->must_exit=true;
            //std::cout<<"curr:0"<<std::endl;
        }
        if (Input::wasKeyPressed(SDL_SCANCODE_2)){
            int end = world->moveTo(levels[0]->islands[1]);
            if(end) Game::instance->must_exit=true;
            //std::cout<<"curr:1"<<std::endl;
        }
        //if (Input::wasKeyPressed(SDL_SCANCODE_3)){
        //    world->boat->current_island = NULL;
        //    //std::cout<<"curr:NULL"<<std::endl;
        //}
    
    if (world->boat->current_island != NULL && world->boat->current_NPC == NULL){
        if (Input::wasKeyPressed(SDL_SCANCODE_7) && world->boat->current_island->npc_vec[0]){
            world->pickup(world->all_npc[0]);
        }
        if (Input::wasKeyPressed(SDL_SCANCODE_8) && world->boat->current_island->npc_vec[1]){
            world->pickup(world->all_npc[1]);
        }
        if (Input::wasKeyPressed(SDL_SCANCODE_9) && world->boat->current_island->npc_vec[2]){
            world->pickup(world->all_npc[2]);
        }
    } 
    if (world->boat->current_island != NULL && world->boat->current_NPC != NULL && Input::wasKeyPressed(SDL_SCANCODE_0)){
        world->drop();
    }
    
    if (world->boat->current_NPC != NULL){
        Vector3 new_pos = 
        Vector3(world->boat->pos.x,
                world->boat->pos.y,
                world->boat->pos.z+1);
        world->boat->current_NPC->pos = new_pos;
    }

    world->boat->mesh->model.setTranslation(world->boat->pos.x,world->boat->pos.y,world->boat->pos.z);
    world->boat->mesh->model.scale(2, 2, 2);
    //world->boat->mesh->model.rotate(PI/2,Vector3(1,1,0));
    std::vector<NPC*> all_npc = world->all_npc;
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
