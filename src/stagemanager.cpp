#include "stagemanager.h"
#include "game.h"
#include "input.h"

MenuStage::MenuStage(void){
    Camera* camera = Game::instance->camera;
    //camera->lookAt(Vector3(0.f, 5.f, 50.f),Vector3(0.f,0.f,20.f), Vector3(0.f,1.f,0.f)); 
    //camera->setPerspective(70.f,Game::instance->window_width/(float)Game::instance->window_height,0.1f,10000.f); 
    camera->lookAt(Vector3(70.f, 65.f, 40.f),Vector3(70.f,1.f,39.f), Vector3(0.f,1.f,0.f)); //TODO TREURE
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
    if (Input::wasKeyPressed(SDL_SCANCODE_RETURN) && option == 1){
        Camera* camera = Game::instance->camera;
        camera->lookAt(Vector3(25.f, 25.f, 50.f),Vector3(0.f,0.f,20.f), Vector3(0.f,1.f,0.f)); 
        camera->setPerspective(70.f,Game::instance->window_width/(float)Game::instance->window_height,0.1f,10000.f); 
        Game::instance->curr_stage = PLAY_STAGE;
    }
}

PlayStage::PlayStage(void){
    world = Game::instance->world;
}

void PlayStage::render(){
    world->renderWorld();
}

void PlayStage::update(float seconds_elapsed){
    
    Camera* camera = Game::instance->camera;

    float speed = seconds_elapsed * Game::instance->mouse_speed;

    bool* mouse_locked = &(Game::instance->mouse_locked);

    //mouse input to rotate the cam
    if ((Input::mouse_state & SDL_BUTTON_LEFT) || *mouse_locked ) //is left button pressed?
    {
        camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f,-1.0f,0.0f));
        camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector( Vector3(-1.0f,0.0f,0.0f)));
    }

    //async input to move the camera around
    if(Input::isKeyPressed(SDL_SCANCODE_LSHIFT) ) speed *= 10; //move faster with left shift
    if (Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
    if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f,-1.0f) * speed);
    if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
    if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f,0.0f, 0.0f) * speed);
    
    /*
    if (Input::isKeyPressed(SDL_SCANCODE_UP)) player->mesh->model.translate(0.0f, 0.0f, -0.1f);
    if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) player->mesh->model.translate(0.0f, 0.0f, 0.1f);
    if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) player->mesh->model.translate(-0.1f, 0.0f, 0.0f);
    if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) player->mesh->model.translate(0.1f,0.0f, 0.0f);
    */

    if (Input::isKeyPressed(SDL_SCANCODE_M)) world->sea->tiling += 1.0;
    if (Input::isKeyPressed(SDL_SCANCODE_N)) world->sea->tiling -= 1.0;
    
    
    Vector2 directions[9] = {Vector2(0,-1), Vector2(1,0), Vector2(-1,0), Vector2(0,1),
                             Vector2(1,-1), Vector2(-1,-1), Vector2(1,1), Vector2(-1,1),
                             Vector2(0,0)};
    int mov_i = 8;

    if (Input::isKeyPressed(SDL_SCANCODE_W)) {
        if(world->boat->current_island->links[0]){
            mov_i = 0;
        }
    }
    if (Input::isKeyPressed(SDL_SCANCODE_D)) {
        if(world->boat->current_island->links[1]){
            mov_i = 1;
        }
    }
    if (Input::isKeyPressed(SDL_SCANCODE_A)) {
        if(world->boat->current_island->links[2]){
            mov_i = 2;
        }
    }
    if (Input::isKeyPressed(SDL_SCANCODE_X)) {
        if(world->boat->current_island->links[3]){
            mov_i = 3;
        }
    }
    if (Input::isKeyPressed(SDL_SCANCODE_E)) {
        if(world->boat->current_island->links[4]){
            mov_i = 4;
        }
    }
    if (Input::isKeyPressed(SDL_SCANCODE_Q)) {
        if(world->boat->current_island->links[5]){
            mov_i = 5;
        }
    }
    if (Input::isKeyPressed(SDL_SCANCODE_C)) {
        if(world->boat->current_island->links[6]){
            mov_i = 6;
        }
    }
    if (Input::isKeyPressed(SDL_SCANCODE_Z)) {
        if(world->boat->current_island->links[7]){
            mov_i = 7;
        }
    }

    Vector2 mov = directions[mov_i];

    //world->boat->pos.x += mov.x * 1;
    //world->boat->pos.z += mov.y * 1;
    bool restart = false;
    if (mov.x!=0 || mov.y!=0){
        bool done = false;
        for (int i=0; i<world->islands.size() && done==false; i++){
            /*BoundingBox b = world->islands[i]->mesh->mesh->box;
            Vector3 coll;
            if (RaySphereCollision(b.center, 50, world->boat->pos, Vector3(mov.x,0,mov.y), coll)){
                world->moveTo(world->islands[i]);
                world->boat->pos = world->islands[i]->pos;
            }*/
            int max = 6;
            int step = 1;
            Vector2 dest = world->islands[i]->tilemap_pos;
            while (step <= max && done == false){
                Vector2 orig = world->boat->current_island->tilemap_pos+mov*step;
                if (orig.x == dest.x && orig.y == dest.y){
                    int end = world->moveTo(world->islands[i]);
                    //std::cout<<end<<std::endl;
                    //if (end!=0) Game::instance->must_exit=true;
                    if (end>0) restart=true;
                    else if (end == 0) world->boat->pos = world->islands[i]->pos;
                    done = true;
                }
                else step+=1;
            }
        }
        //std::cout<<"NORTH, EAST, WEST, SOUTH, NORTHEAST, NORTHWEST, SOUTHEAST, SOUTHWEST"<<std::endl;
        //for(int d=0; d<8; d++){std::cout<<world->boat->current_island->links[d]<<std::endl;}
    }

    //if (end) Game::instance->must_exit=true;
    
    //TO PRINT INFO ABOUT CURRENT ISLAND AND NPC
        //if (world->boat->current_NPC != NULL) std::cout<<"n:"<<world->boat->current_NPC->type<<std::endl;
        //if (world->boat->current_island != NULL) std::cout<<"i:"<<world->boat->current_island->type<<std::endl;
    
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
    if (Input::wasKeyPressed(SDL_SCANCODE_C)) world->boat->movesAlone = 0;
    if (Input::wasKeyPressed(SDL_SCANCODE_R)) restart = true;

    if(restart){world->setup_level(world->gamemap);}
}

void PauseStage::render(){

}

void PauseStage::update(float seconds_elapsed){
    if (Input::wasKeyPressed(SDL_SCANCODE_P)) Game::instance->curr_stage = PLAY_STAGE;
}
