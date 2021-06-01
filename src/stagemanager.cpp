#include "stagemanager.h"
#include "game.h"
#include "input.h"

void moveCamera(Vector3 eye_dest, Vector3 center_dest, float coeff){
    Camera* c = Game::instance->camera;
    //float coeff = 0.01;
    if(c->eye.x != eye_dest.x || c->eye.y != eye_dest.y || c->eye.z != eye_dest.z){
        c->eye = c->eye + (eye_dest - c->eye) * coeff;
    }
    if((c->center.x != center_dest.x || c->center.y != center_dest.y) || c->center.z != center_dest.z){
        c->center = c->center + (center_dest - c->center) * coeff;
    }
}

MenuStage::MenuStage(void){
    Camera* camera = Game::instance->camera;
    camera->lookAt(Vector3(-11.4899, 2.53442, 3.4166),Vector3(-44.9116, -13.6219, -48.7266), Vector3(0.f,1.f,0.f));
    //camera->setPerspective(70.f,Game::instance->window_width/(float)Game::instance->window_height,0.1f,10000.f);
    
    Mesh* mesh_play = Mesh::Get("data/assets/Font/play.obj");
    Mesh* mesh_quit = Mesh::Get("data/assets/Font/quit.obj");
    
    Texture* tex = new Texture();
    tex->load("data/assets/color-atlas-new.tga");
    
    play_button = new EntityMesh(mesh_play, tex, Shader::Get("data/shaders/hover.vs", "data/shaders/illumination.fs"), Vector4(1,1,1,1));
    play_button->model.translate(-12.17, 2.33, 1);
    
    quit_button = new EntityMesh(mesh_quit, tex, Shader::Get("data/shaders/basic.vs", "data/shaders/illumination.fs"), Vector4(0.3,0.3,0.3,1));
    quit_button->model.translate(-12.57, 1.13, 0);
    
    selected = PLAY_BUTTON;
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
    //em->render();
    
    play_button->render();
    quit_button->render();
}

void MenuStage::update(float seconds_elapsed){

    // SELECT PLAY BUTTON
    if (Input::wasKeyPressed(SDL_SCANCODE_UP) && selected == QUIT_BUTTON){
        play_button->model.translate(0,0,1);
        quit_button->model.translate(0,0,-1);
        selected = PLAY_BUTTON;
        
        play_button->shader = Shader::Get("data/shaders/hover.vs", "data/shaders/illumination.fs");
        quit_button->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/illumination.fs");
        
        play_button->color = Vector4(1,1,1,1);
        quit_button->color = Vector4(0.3, 0.3, 0.3, 1);
    }
    
    // SELECT QUIT BUTTON
    if (Input::wasKeyPressed(SDL_SCANCODE_DOWN) && selected == PLAY_BUTTON){
        play_button->model.translate(0,0,-1);
        quit_button->model.translate(0,0,1);
        selected = QUIT_BUTTON;
        
        quit_button->shader = Shader::Get("data/shaders/hover.vs", "data/shaders/illumination.fs");
        play_button->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/illumination.fs");
        
        quit_button->color = Vector4(1,1,1,1);
        play_button->color = Vector4(0.3, 0.3, 0.3, 1);
    }
    
    // PRESS SELECTED BUTTON
    if (Input::wasKeyPressed(SDL_SCANCODE_RETURN)){
        if(selected == QUIT_BUTTON) Game::instance->must_exit = true;
        else{
            Camera* camera = Game::instance->camera;
            camera->lookAt(Vector3(70.f, 65.f, 40.f),Vector3(70.f,1.f,39.f), Vector3(0.f,1.f,0.f));
            //camera->lookAt(Vector3(25.f, 25.f, 50.f),Vector3(0.f,0.f,20.f), Vector3(0.f,1.f,0.f));
            //camera->setPerspective(70.f,Game::instance->window_width/(float)Game::instance->window_height,0.1f,10000.f);
            Game::instance->curr_stage = PLAY_STAGE;
        }
    }
    
}

PlayStage::PlayStage(void){
    world = Game::instance->world;
}

void PlayStage::render(){
    world->renderWorld();
}

void PlayStage::update(float seconds_elapsed){

    bool restart = false;
    
    Camera* camera = Game::instance->camera;

    float speed = seconds_elapsed * Game::instance->mouse_speed;

    bool* mouse_locked = &(Game::instance->mouse_locked);

    //mouse input to rotate the cam
    if ((Input::mouse_state & SDL_BUTTON_LEFT) || *mouse_locked ) //is left button pressed?
    {
        camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f,-1.0f,0.0f));
        camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector( Vector3(-1.0f,0.0f,0.0f)));
        std::cout<<"iau"<<std::endl;
        //reconstruct world position from depth and inv. viewproj //TODO: CLICK!
        /*float depth = texture( u_depth_texture, uv ).x; 
        vec4 screen_pos = vec4(uv.x*2.0-1.0, uv.y*2.0-1.0, depth*2.0-1.0, 1.0);
        vec4 proj_worldpos = u_inverse_viewprojection * screen_pos;
        vec3 worldpos = proj_worldpos.xyz / proj_worldpos.w;*/
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
    
    /*if (Input::wasKeyPressed(SDL_MOUSEBUTTONDOWN)){
        std::cout<<"iau"<<std::endl;
    }*/

    Vector2 directions[9] = {Vector2(0,-1), Vector2(1,0), Vector2(-1,0), Vector2(0,1),
                             Vector2(1,-1), Vector2(-1,-1), Vector2(1,1), Vector2(-1,1),
                             Vector2(0,0)};
    SDL_Scancode direction_keys[8] = {SDL_SCANCODE_W,SDL_SCANCODE_D,SDL_SCANCODE_A,SDL_SCANCODE_X,SDL_SCANCODE_E,SDL_SCANCODE_Q,SDL_SCANCODE_C,SDL_SCANCODE_Z};
    int mov_i = 8;
    if (world->boat->moving.x == 0 && world->boat->moving.y == 0){
    
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

        for (int d = 0; d<8; d++){
            if (Input::isKeyPressed(direction_keys[d]) && world->boat->current_island->links[d]) mov_i = d; 
        }
        
        if (mov_i!=8){
            Island* dest = world->boat->current_island->links[mov_i];
            int end = world->moveTo(dest);
            if (end>0) restart=true;
            else if (end == 0) {
                //world->boat->pos = dest->pos;
                world->boat->moving = directions[mov_i];
            }
        }
        else{moveCamera(Vector3(70.f, 65.f, 40.f),Vector3(70.f,1.f,39.f),0.1);}
    }
    else {
        world->boat->pos = world->boat->pos + Vector3(world->boat->moving.x, 0, world->boat->moving.y);

        if (world->boat->current_NPC != NULL){
            Vector3 new_pos = 
            Vector3(world->boat->pos.x,
                    world->boat->pos.y,
                    world->boat->pos.z+1);
            world->boat->current_NPC->pos = new_pos;
        }

        if (world->boat->pos.x == world->boat->current_island->pos.x && world->boat->pos.z == world->boat->current_island->pos.z){
            world->boat->moving = Vector2(0,0);
            std::cout<<"stop"<<std::endl;
        }
        else if (world->moving_track)
            //moveCamera(world->boat->pos+Vector3(0,65,0),world->boat->pos,0.01);
            moveCamera(world->boat->pos+Vector3(-15*world->boat->moving.x, 15, -15*world->boat->moving.y),world->boat->pos,0.01);
            //Game::instance->camera->lookAt(world->boat->pos+Vector3(-15*world->boat->moving.x, 15, -15*world->boat->moving.y),world->boat->pos, Vector3(0.f,1.f,0.f));

    }

    world->boat->mesh->model.setTranslation(world->boat->pos.x,world->boat->pos.y,world->boat->pos.z);
    //world->boat->mesh->model.rotate(atan(world->boat->moving.y/world->boat->moving.x),Vector3(0,1,0)); // I WAS UNABLE TO PROPERLY ROTATE THE BOAT, FOR NOW jeje
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
    if (Input::wasKeyPressed(SDL_SCANCODE_T)) world->boat->movesAlone = 0;
    if (Input::wasKeyPressed(SDL_SCANCODE_R)) restart = true;

    if(restart){world->setup_level(world->gamemap);}
}

void PauseStage::render(){

}

void PauseStage::update(float seconds_elapsed){
    if (Input::wasKeyPressed(SDL_SCANCODE_P)) Game::instance->curr_stage = PLAY_STAGE;
    if (Input::wasKeyPressed(SDL_SCANCODE_M)){
        Game::instance->camera->lookAt(Vector3(-11.4899, 2.53442, 3.4166),Vector3(-44.9116, -13.6219, -48.7266), Vector3(0.f,1.f,0.f));
        Game::instance->curr_stage = MENU_STAGE;
    } 
}
