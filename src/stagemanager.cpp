#include "stagemanager.h"
#include "game.h"
#include "input.h"
#include "audio.h"

void Stage::sceneMaker(EntityMesh* to_move, float speed){
    if (to_move != nullptr){
        if (Input::isKeyPressed(SDL_SCANCODE_W)) to_move->model.translate(0.0f, 0.0f, -speed);
        if (Input::isKeyPressed(SDL_SCANCODE_S)) to_move->model.translate(0.0f, 0.0f, speed);
        if (Input::isKeyPressed(SDL_SCANCODE_A)) to_move->model.translate(-speed, 0.0f, 0.0f);
        if (Input::isKeyPressed(SDL_SCANCODE_D)) to_move->model.translate(speed,0.0f, 0.0f);
        if (Input::isKeyPressed(SDL_SCANCODE_E)) to_move->model.translate(0.0f,speed, 0.0f);
        if (Input::isKeyPressed(SDL_SCANCODE_Q)) to_move->model.translate(0.0f,-speed, 0.0f);
        if (Input::isKeyPressed(SDL_SCANCODE_F)) {to_move->model.rotate(speed, Vector3(0,1,0)); acumulated_rotation += speed;}
        if (Input::isKeyPressed(SDL_SCANCODE_G)) {to_move->model.rotate(-speed, Vector3(0,1,0)); acumulated_rotation -= speed;}
        if (Input::isKeyPressed(SDL_SCANCODE_Z)) to_move->model.scale(0.9, 0.9, 0.9);
        
        if(Input::wasKeyPressed(SDL_SCANCODE_L)){
            std::cout << acumulated_rotation << std::endl;
            std::cout << to_move->model.getTranslation().x << ", " << to_move->model.getTranslation().y << ", " << to_move->model.getTranslation().z << std::endl;
        }
    }
    Camera* camera = Game::instance->camera;
    if ((Input::mouse_state & SDL_BUTTON_LEFT)) //is left button pressed?
     {
         camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f,-1.0f,0.0f));
         camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector( Vector3(-1.0f,0.0f,0.0f)));
     }
    
    if (Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f));
    if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f,-1.0f));
    if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f));
    if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f,0.0f, 0.0f));
}

bool moveCamera(Vector3 eye_dest, Vector3 center_dest, Vector3 up_dest, float coeff){
    Camera* c = Game::instance->camera;
    //float coeff = 0.01;
    bool return_bool = false;
    if(std::abs(c->eye.x - eye_dest.x) > 0.000001|| std::abs(c->eye.y - eye_dest.y) > 0.000001|| std::abs(c->eye.z - eye_dest.z) > 0.000001){
        c->eye = c->eye + (eye_dest - c->eye) * coeff;
        return_bool=true;
    }
    if(std::abs(c->center.x - center_dest.x) > 0.000001|| std::abs(c->center.y - center_dest.y) > 0.000001 || std::abs(c->center.z - center_dest.z) > 0.000001){
        c->center = c->center + (center_dest - c->center) * coeff;
        return_bool=true;
    }
    /*if(c->up.x - up_dest.x > 0.0001 || c->up.y - up_dest.y > 0.0001 || c->up.z - up_dest.z > 0.0001){
        c->up = c->up + (up_dest - c->up) * coeff;
    }*/
    c->up = up_dest;
    return return_bool;
}

MenuStage::MenuStage(void){
    Camera* camera = Game::instance->camera;
    camera->lookAt(Vector3(-11.4899, 2.53442, 3.4166),Vector3(-44.9116, -13.6219, -48.7266), Vector3(0.f,1.f,0.f));
    //camera->setPerspective(70.f,Game::instance->window_width/(float)Game::instance->window_height,0.1f,10000.f);
    
    Shader* s_hover = Shader::Get("data/shaders/hover.vs", "data/shaders/illumination.fs");
    Shader* s_basic = Shader::Get("data/shaders/basic.vs", "data/shaders/illumination.fs");
    Shader* s_wind = Shader::Get("data/shaders/wind.vs", "data/shaders/illumination.fs");
    
    Mesh* mesh_play = Mesh::Get("data/assets/Font/play.obj");
    Mesh* mesh_quit = Mesh::Get("data/assets/Font/quit.obj");
    Mesh* mesh_island = Mesh::Get("data/assets/Island/terrain-mountain-range_1.obj");
    Mesh* mesh_boat = Mesh::Get("data/assets/Boat/boat.obj");
    Mesh* mesh_flower1 = Mesh::Get("data/assets/Flowers/carnations_red.obj");
    Mesh* mesh_flower2 = Mesh::Get("data/assets/Flowers/roses_red.obj");
    Mesh* mesh_flower3 = Mesh::Get("data/assets/Flowers/carnations_lightblue.obj");
    Mesh* mesh_flower4 = Mesh::Get("data/assets/Flowers/carnations_pink.obj");
    Mesh* mesh_flower5 = Mesh::Get("data/assets/Flowers/carnations_yellow.obj");
    Mesh* mesh_flower6 = Mesh::Get("data/assets/Flowers/roses_blue.obj");
    Mesh* mesh_flower7 = Mesh::Get("data/assets/Flowers/roses_orange.obj");
    Mesh* mesh_flower8 = Mesh::Get("data/assets/Flowers/roses_purple.obj");
    Mesh* mesh_flower9 = Mesh::Get("data/assets/Flowers/roses_yellow.obj");
    
   
    Texture* tex = new Texture();
    tex->load("data/assets/color-atlas-new.tga");
    
    play_button = new EntityMesh(mesh_play, tex, s_hover, Vector4(1,1,1,1));
    play_button->model.translate(-12.17, 2.33, 1);
    
    quit_button = new EntityMesh(mesh_quit, tex, s_basic, Vector4(0.3,0.3,0.3,1));
    quit_button->model.translate(-12.57, 1.13, 0);
    
    island = new EntityMesh(mesh_island, tex, s_basic, Vector4(1,1,1,1));
    island->model.translate(-22.3277, 0.3, -8.81457);
    island->model.scale(0.5, 0.5, 0.5);
    island->model.rotate(-1.46, Vector3(0, 1, 0));
    
    boat = new EntityMesh(mesh_boat, tex, s_hover, Vector4(1,1,1,1));
    boat->model.translate(-16.6868, 0.3, -25.7556);
    boat->model.rotate(11.7402, Vector3(0, 1, 0));
    
    flowers.push_back( new EntityMesh(mesh_flower1, tex, s_wind, Vector4(1,1,1,1)));
    flowers[0]->model.translate(-16.1, 0.5, 0.2);
    
    flowers.push_back( new EntityMesh(mesh_flower2, tex, s_wind, Vector4(1,1,1,1)));
    flowers[1]->model.translate(-28.4, 7.1, -9.7);
    
    flowers.push_back( new EntityMesh(mesh_flower3, tex, s_wind, Vector4(1,1,1,1)));
    flowers[2]->model.translate(-19.9, 0.5, -0.2);
    flowers[2]->model.rotate(-1.15, Vector3(0, 1, 0));
    
    flowers.push_back( new EntityMesh(mesh_flower4, tex, s_wind, Vector4(1,1,1,1)));
    flowers[3]->model.translate(-11, 0.5, -4);
    
    flowers.push_back( new EntityMesh(mesh_flower5, tex, s_wind, Vector4(1,1,1,1)));
    flowers[4]->model.translate(-15.4702, 0.5, 2.24205);
    flowers[4]->model.rotate(-3.16, Vector3(0, 1, 0));
    
    flowers.push_back( new EntityMesh(mesh_flower6, tex, s_wind, Vector4(1,1,1,1)));
    flowers[5]->model.translate(-11.7702, 0.5, 1.54205);
    
    flowers.push_back( new EntityMesh(mesh_flower7, tex, s_wind, Vector4(1,1,1,1)));
    flowers[6]->model.translate(-15.8286, 0.5, -4.32644);
    flowers[6]->model.rotate(-1.41, Vector3(0, 1, 0));
    
    flowers.push_back( new EntityMesh(mesh_flower8, tex, s_wind, Vector4(1,1,1,1)));
    flowers[7]->model.translate(-14.0718, 0.5, -9.40225);
    flowers[7]->model.rotate(0.67, Vector3(0, 1, 0));
    
    flowers.push_back( new EntityMesh(mesh_flower9, tex, s_wind, Vector4(1,1,1,1)));
    flowers[8]->model.translate(-24.7064, 4, -13.8823);
    flowers[8]->model.rotate(0.06, Vector3(0, 1, 0));
    
    flowers.push_back( new EntityMesh(mesh_flower4, tex, s_wind, Vector4(1,1,1,1)));
    flowers[9]->model.translate(-29.3001, 5.98794, -4.61804);
    flowers[9]->model.rotate(-0.46, Vector3(1, 0, 0));
    
    flowers.push_back( new EntityMesh(mesh_flower6, tex, s_wind, Vector4(1,1,1,1)));
    flowers[10]->model.translate(-24.8702, 1.7, -4.95795);
    
    selected = PLAY_BUTTON;

}

void MenuStage::render(){
    Camera* camera = Game::instance->camera;
    
    Game::instance->world->sky->model.setTranslation(camera->eye.x,camera->eye.y,camera->eye.z);
    Game::instance->world->sky->render();
    Game::instance->world->renderWater();
    
    play_button->render();
    quit_button->render();
    island->render();
    boat->render();
    
    for(int i = 0; i < flowers.size(); i++){
        flowers[i]->render();
    }
    
}

void MenuStage::update(float seconds_elapsed){

    Game::instance->camera->lookAt(Vector3(-11.4899, 2.53442, 3.4166),Vector3(-44.9116, -13.6219, -48.7266), Vector3(0.f,1.f,0.f));
    boat->model.rotate(sin(Game::instance->time)*0.002, Vector3(0, 0, 1));
    
    // SELECT PLAY BUTTON
    if (Input::wasKeyPressed(SDL_SCANCODE_UP) && selected == QUIT_BUTTON){
        play_button->model.translate(0,0,1);
        quit_button->model.translate(0,0,-1);
        selected = PLAY_BUTTON;
        
        play_button->shader = Shader::Get("data/shaders/hover.vs", "data/shaders/illumination.fs");
        quit_button->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/illumination.fs");
        
        play_button->color = Vector4(1,1,1,1);
        quit_button->color = Vector4(0.3, 0.3, 0.3, 1);

        Audio::Play("data/assets/Sound/menu_nav.wav");
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

        Audio::Play("data/assets/Sound/menu_nav.wav");
    }
    
    // PRESS SELECTED BUTTON
    if (Input::wasKeyPressed(SDL_SCANCODE_RETURN)){
        if(selected == QUIT_BUTTON) Game::instance->must_exit = true;
        else{
            Audio::Play("data/assets/Sound/menu_ok.wav");
            Camera* camera = Game::instance->camera;
            //camera->lookAt(Vector3(70.f, 65.f, 40.f),Vector3(70.f,1.f,39.f), Vector3(0.f,1.f,0.f));
            //camera->lookAt(Vector3(25.f, 25.f, 50.f),Vector3(0.f,0.f,20.f), Vector3(0.f,1.f,0.f));
            //camera->setPerspective(70.f,Game::instance->window_width/(float)Game::instance->window_height,0.1f,10000.f);
            Game::instance->curr_stage = PLAY_STAGE;
            PlayStage* ps = (PlayStage*)(Game::instance->stages[PLAY_STAGE]);
            ps->current_level = 0;
            ps->world->setup_level(ps->levels[ps->current_level]);
            Audio::Play("data/assets/Sound/music.wav", 4);
        }
    }
    
    // MENU MAKER
    //sceneMaker(flowers[0], 0.1);
    
}

PlayStage::PlayStage(void){
    world = Game::instance->world;
    
    int num_instructions = 7;
    Texture* t_instructions[num_instructions];
    for(int i = 0; i < num_instructions; i++){
        t_instructions[i] = new Texture();
    }
    Mesh* mesh_instructions = Mesh::getQuad();

    Shader* s_instructions = Shader::Get("data/shaders/quad.vs", "data/shaders/texture.fs");
    t_instructions[0]->load("data/assets/instructions/instructions_quim_30.tga");
    t_instructions[1]->load("data/assets/instructions/wolves_30.tga");
    t_instructions[2]->load("data/assets/instructions/shepherd_30.tga");
    t_instructions[3]->load("data/assets/instructions/plague_30.tga");
    t_instructions[4]->load("data/assets/instructions/bears_30.tga");
    t_instructions[5]->load("data/assets/instructions/party_30.tga");
    t_instructions[6]->load("data/assets/instructions/controls_30.tga");
    //t_instructions[7]->load("data/assets/instructions/c1.tga");
    
    instructions_quads.push_back(new EntityMesh(mesh_instructions, t_instructions[0], s_instructions, Vector4(1, 1, 1, 1)));
    instructions_quads.push_back(new EntityMesh(mesh_instructions, t_instructions[1], s_instructions, Vector4(1, 1, 1, 1)));
    instructions_quads.push_back(new EntityMesh(mesh_instructions, t_instructions[2], s_instructions, Vector4(1, 1, 1, 1)));
    instructions_quads.push_back(new EntityMesh(mesh_instructions, t_instructions[3], s_instructions, Vector4(1, 1, 1, 1)));
    instructions_quads.push_back(new EntityMesh(mesh_instructions, t_instructions[4], s_instructions, Vector4(1, 1, 1, 1)));
    instructions_quads.push_back(new EntityMesh(mesh_instructions, t_instructions[5], s_instructions, Vector4(1, 1, 1, 1)));
    instructions_quads.push_back(new EntityMesh(mesh_instructions, t_instructions[6], s_instructions, Vector4(1, 1, 1, 1)));
    //instructions_quads.push_back(new EntityMesh(mesh_instructions, t_instructions[7], s_instructions, Vector4(1, 1, 1, 1)));
    
    instructions_index = 0;

    Texture* t_return = new Texture();
    t_return->load("data/assets/UI/return.tga");
    Mesh* mesh_return = new Mesh();
		mesh_return->createQuad(0.75, -0.75, 0.3, 0.3, false);
		mesh_return->uploadToVRAM();
    return_quad = new EntityMesh(mesh_return, t_return, s_instructions, Vector4(1, 1, 1, 1));


    Texture* t_heart = new Texture();
    t_heart->load("data/assets/UI/heart.tga");
    for(int h=0; h<3; h++){
        Mesh* mesh_heart = new Mesh();
		mesh_heart->createQuad(-0.80+h*0.25, -0.75, 0.2, 0.2, false);
		mesh_heart->uploadToVRAM();
        heart_quads.push_back(new EntityMesh(mesh_heart, t_heart, s_instructions, Vector4(3, 0.5, 1, 1)));
    }
    
}

void PlayStage::render(){
    world->renderWorld();
    
    glEnable(GL_BLEND);
        if(show_instructions) instructions_quads[instructions_index]->render();
        else{
            for(int h=0; h<world->boat->lives; h++){
                heart_quads[h]->render();
            }
            if(show_instructions) instructions_quads[instructions_index]->render();
            if(show_return) return_quad->render();
        }
    glDisable(GL_BLEND);
}

void PlayStage::update(float seconds_elapsed){
    
    // SHOW INSTRUCTIONS SCREEN WHEN PRESSING TAB
    if(Input::wasKeyPressed(SDL_SCANCODE_TAB)) {
        instructions_index = 0;
        if(show_instructions) show_instructions = false;
        else show_instructions = true;
        Audio::Play("data/assets/Sound/menu_nav.wav");
    }
    //if(Input::wasKeyPressed(SDL_SCANCODE_RETURN)) show_instructions = false;
    // the game pauses when instructions are showing
    if(show_instructions){
        if (Input::wasKeyPressed(SDL_SCANCODE_RIGHT)){
            if(instructions_index == instructions_quads.size() - 1) show_instructions = false;
            else instructions_index++;
            Audio::Play("data/assets/Sound/menu_nav.wav");
        }
        if (Input::wasKeyPressed(SDL_SCANCODE_LEFT)){
            if(instructions_index == 0) show_instructions = false;
            else instructions_index--;
            Audio::Play("data/assets/Sound/menu_nav.wav");
        }
        if (Input::wasKeyPressed(SDL_SCANCODE_RSHIFT)){
            Audio::Play("data/assets/Sound/menu_ok.wav");
            current_level += 1;
            if (current_level >= levels.size()){
                Game::instance->camera->lookAt(Vector3(-11.4899, 2.53442, 3.4166),Vector3(-44.9116, -13.6219, -48.7266), Game::instance->camera->up);
                Game::instance->curr_stage = CREDITS_STAGE;
                CreditsStage* cs = (CreditsStage*)(Game::instance->stages[CREDITS_STAGE]);
                cs->setupMovingCredits();
                Audio::Play("data/assets/Sound/win.wav");
                Audio::Stop("data/assets/Sound/music.wav");
            }
            else{
                //Game::instance->camera->lookAt(Vector3(70.f, 65.f, 40.f),Vector3(70.f,1.f,39.f), Game::instance->camera->up);
                world->setup_level(levels[current_level]);
                show_instructions = false;
                Audio::Play("data/assets/Sound/music.wav", 4);
            }
        } 
        if (Input::wasKeyPressed(SDL_SCANCODE_M)){
            Game::instance->camera->lookAt(Vector3(-11.4899, 2.53442, 3.4166),Vector3(-44.9116, -13.6219, -48.7266), Game::instance->camera->up);
            Game::instance->curr_stage = MENU_STAGE;
            show_instructions = false;
            //Audio::Play("data/assets/Sound/waves.wav");
            Audio::Play("data/assets/Sound/menu_ok.wav");
        } 
    return;
    }

    bool restart = false;
    show_return = false;
    
    Camera* camera = Game::instance->camera;

    float speed = seconds_elapsed * Game::instance->mouse_speed;

    bool* mouse_locked = &(Game::instance->mouse_locked);

    //mouse input to rotate the cam

    int npccursor = -1;
    int islandcursor = -1;
    int boatcursor = -1;
    if (Input::isMousePressed(SDL_BUTTON_LEFT) || *mouse_locked ) //is left button pressed?
    {
        if(world->closeview){
            camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f,-1.0f,0.0f));
            camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector( Vector3(-1.0f,0.0f,0.0f)));
        }
        //std::cout<<"iau"<<std::endl;
        if(Input::clicked){
            Vector3 mouseRay = Game::instance->camera->getRayDirection(
                Input::mouse_position.x, Input::mouse_position.y,
                Game::instance->window_width, Game::instance->window_height
            );

            //Vector3 collz;
            Vector3 coll;
            Vector3 normal;
            bool found = false;
            for(int i = 0;i<world->all_npc.size();i++){
                found = world->all_npc[i]->mesh->mesh->testRayCollision(
                    world->all_npc[i]->mesh->model,
                    Game::instance->camera->eye,mouseRay,
                    coll,normal
                );
                if(found){npccursor = i;}
            }
            if(!found){
                for(int i = 0;i<world->islands.size();i++){
                    found = world->islands[i]->mesh->mesh->testRayCollision(
                        world->islands[i]->mesh->model,
                        Game::instance->camera->eye,mouseRay,
                        coll,normal
                    );
                    if(found){islandcursor = i;}
                }
            }
            if(!found){
                found = world->boat->mesh->mesh->testRayCollision(
                    world->boat->mesh->model,
                    Game::instance->camera->eye,mouseRay,
                    coll,normal
                );
                if(found){boatcursor = 1;}
            }
            Input::clicked = false;
        }
    }

    //async input to move the camera around
    /*if(Input::isKeyPressed(SDL_SCANCODE_LSHIFT) ) speed *= 10; //move faster with left shift
    if (Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
    if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f,-1.0f) * speed);
    if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
    if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f,0.0f, 0.0f) * speed);*/
    
    /*
    if (Input::isKeyPressed(SDL_SCANCODE_UP)) player->mesh->model.translate(0.0f, 0.0f, -0.1f);
    if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) player->mesh->model.translate(0.0f, 0.0f, 0.1f);
    if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) player->mesh->model.translate(-0.1f, 0.0f, 0.0f);
    if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) player->mesh->model.translate(0.1f,0.0f, 0.0f);
    */

    //if (Input::isKeyPressed(SDL_SCANCODE_M)) world->sea->tiling += 1.0;
    //if (Input::isKeyPressed(SDL_SCANCODE_N)) world->sea->tiling -= 1.0;
    
    /*if (Input::wasKeyPressed(SDL_MOUSEBUTTONDOWN)){
        std::cout<<"iau"<<std::endl;
    }*/

    Vector2 directions[9] = {Vector2(0,-1), Vector2(1,0), Vector2(-1,0), Vector2(0,1),
                             Vector2(1,-1), Vector2(-1,-1), Vector2(1,1), Vector2(-1,1),
                             Vector2(0,0)};
    int angles[8] = {4, 6, 2, 0, 5, 3, 7, 1};
    //SDL_Scancode direction_keys[8] = {SDL_SCANCODE_W,SDL_SCANCODE_D,SDL_SCANCODE_A,SDL_SCANCODE_X,SDL_SCANCODE_E,SDL_SCANCODE_Q,SDL_SCANCODE_C,SDL_SCANCODE_Z};
    int mov_i = 8;
    t += seconds_elapsed*0.6;
    if (world->boat->moving.x == 0 && world->boat->moving.y == 0){
        
        //std::cout<<t<<std::endl;
        float radius = 18+cos(5*t)*sin(2*t);
        //Vector3 circling = world->boat->circling;
        world->boat->pos = world->boat->current_island->pos + radius*Vector3(cos(t),0,sin(t));
        //std::cout<<circling.x<<", "<<circling.y<<", "<<circling.z<<std::endl;
        //world->boat->mesh->model.setTranslation(world->boat->pos.x,world->boat->pos.y,world->boat->pos.z);
        //world->boat->mesh->model.rotate(t,Vector3(0,1,0));
        //world->boat->mesh->model.scale(1, 1, 1);

        int able = true;
            bool npc_in_island = false;
            for (int n=0; n<3; n++){
                if(world->boat->current_island->npc_vec[n]==1){npc_in_island=true;}
            }
            if (npc_in_island){
                world->boat->movesAlone = 0;
                if (npccursor != -1 && world->boat->current_island->npc_vec[npccursor]){
                    if(world->boat->current_NPC != NULL) world->drop();
                    world->pickup(world->all_npc[npccursor]);
                    able = false;
                }
            }
            else{
                if(world->boat->current_NPC == NULL){
                    show_return = true;
                    if (Input::wasKeyPressed(SDL_SCANCODE_Z) && world->boat->current_NPC == NULL){
                        Audio::Play("data/assets/Sound/ctrlZ.wav");
                        world->boat->current_island = world->boat->previous_island;
                        world->boat->pos = world->boat->current_island->pos + radius*Vector3(cos(t),0,sin(t)); //TODO or not "Circling"
                        world->boat->movesAlone = 0;
                    }
                }
            }
        //} 
        if (world->boat->current_NPC != NULL && able){
            //std::cout<<world->boat->current_NPC->type<<std::endl;
            //if (Input::wasKeyPressed(SDL_SCANCODE_0)) world->drop();
            if (boatcursor == 1) world->drop();
            else if (npccursor == int(world->boat->current_NPC->type)) world->drop();
        } 

        if (world->closeview && Input::wasKeyPressed(SDL_SCANCODE_RETURN)){
            for (int d = 0; d<8; d++){
                if (world->boat->current_island->links[d]){
                    if(world->boat->current_island->links[d]->index_inVector==world->close_focus) mov_i = d; 
                }
            } 
        }
        else if ((islandcursor!=-1 || npccursor!=-1)&&able){
            if (islandcursor == world->boat->current_island->index_inVector && world->boat->current_NPC) world->drop();
            else{
                for (int d = 0; d<8; d++){
                    if (world->boat->current_island->links[d]){
                        if (islandcursor!=-1){
                            if(world->boat->current_island->links[d]->index_inVector==islandcursor) mov_i = d; 
                        }
                        else if (npccursor!=-1){
                            if(world->boat->current_island->links[d]->npc_vec[npccursor]) mov_i = d; 
                        }
                    }
                }
            }
        }
        
        if (mov_i!=8){
            Island* orig = world->boat->current_island;
            Island* dest = world->boat->current_island->links[mov_i];
            int end = world->moveTo(dest);
            if (end != 0) {
                if (end == 1) world->boat->lives-=1; 
                Audio::Play("data/assets/Sound/bad.wav");
                world->boat->hurt = 10.0;}//restart=true; 
            if (end == 0){
                //world->boat->pos = dest->pos;
                //Audio::Play("data/assets/Sound/sailing.wav");
                Audio::Play("data/assets/Sound/woosh.wav");
                world->boat->moving = directions[mov_i];
                world->boat->mov_ind = mov_i;
                world->boat->pos = orig->pos; //TODO (or not) RELATED TO "CIRCLING"
            }
        }
        

        world->birdview=false;
        //world->closeview=false;
        if (Input::isKeyPressed(SDL_SCANCODE_B)){world->birdview=true;}
        else if (Input::wasKeyPressed(SDL_SCANCODE_C)&&world->closeview){world->closeview=false;}
        else if (Input::wasKeyPressed(SDL_SCANCODE_C)&&!world->closeview){world->closeview=true; world->close_focus = world->boat->current_island->index_inVector;}

        if (world->closeview){
            if (Input::wasKeyPressed(SDL_SCANCODE_LEFT)){world->close_focus = world->close_focus-1; if(world->close_focus<0){world->close_focus=world->islands.size()-1;}}//std::cout<<world->close_focus<<"/"<<world->islands.size()<<std::endl;}
            else if (Input::wasKeyPressed(SDL_SCANCODE_RIGHT)){world->close_focus = (world->close_focus+1)%(world->islands.size());}//std::cout<<world->close_focus<<"/"<<world->islands.size()<<std::endl;}
        }
        /*if (Input::wasKeyPressed(SDL_SCANCODE_P)){
            for(int i =0;i<world->islands.size();i++){
                std::cout<<i<<": "<<world->islands[i]->pos.x<<", "<<world->islands[i]->pos.y<<", "<<world->islands[i]->pos.z<<std::endl;
            }
            std::cout<<world->close_focus<<"/"<<world->islands.size()<<std::endl;
            std::cout<<"EYE:" << Game::instance->camera->eye.x<< " " << Game::instance->camera->eye.y<<" "<< Game::instance->camera->eye.z <<std::endl;
            std::cout<<"CENTER:" << Game::instance->camera->center.x<< " "<< Game::instance->camera->center.y<<" "<< Game::instance->camera->center.z <<std::endl;
        }*/
        
        Vector3 f = Vector3(0,0,0);
        if (current_level >= levels.size()-3){f = Vector3(10.f,1.f,5.f);}
        
        if (world->boat->hurt <= 0){
            if (world->birdview){
                moveCamera(Vector3(70.f, 80.f, 40.f)+f*Vector3(1,10,0),Vector3(70.f,1.f,39.f)+f*Vector3(1,0,0),Vector3(0,1,0),0.1);}
            else if (world->closeview){
                if (!Input::isMousePressed(SDL_BUTTON_LEFT)){moveCamera(world->islands[world->close_focus]->pos+Vector3(0.f, 5.f, 20.f),world->islands[world->close_focus]->pos,Vector3(0,1,0),0.2);}
            }
            else{
                if (current_level == 0){f = Vector3(0.f,-15.f,-25.f);}
                moveCamera(Vector3(70.f, 45.f, 105.f)+f,Vector3(70.f,-5.f,45.f)+f,Vector3(0,1,0),0.1);}//std::cout<<world->boat->hurt<<std::endl;}
            //std::cout<<world->boat->hurt<<std::endl;
        }
        
        //world->boat->mesh->shader = Shader::Get("data/shaders/hover.vs", "data/shaders/illumination.fs");
    }
    else {
        world->boat->pos = world->boat->pos + Vector3(world->boat->moving.x, 0, world->boat->moving.y);

        

        if (world->boat->pos.distance(world->boat->current_island->pos) < 3){//< 21){//TODO (or not) RELATED TO "CIRCLING"
            world->boat->moving = Vector2(0,0);
            //std::cout<<"stop"<<std::endl;
        }
        else if (world->moving_track && world->boat->hurt <= 0 && world->birdview==false && world->closeview==false)
            moveCamera(camera->eye,world->boat->current_island->pos,camera->up,0.1);

        //world->boat->mesh->model.setTranslation(world->boat->pos.x,world->boat->pos.y,world->boat->pos.z);
        //world->boat->mesh->model.rotate(angles[world->boat->mov_ind]*(PI/4),Vector3(0,1,0));
    
    }
    
    if (world->boat->hurt>0.0){
        if(! world->birdview)Game::instance->camera->eye = Game::instance->camera->eye + Vector3(int(world->boat->hurt)%5-2,0,0);
        else Game::instance->camera->eye = Game::instance->camera->eye + Vector3(0,int(world->boat->hurt)%5-2,0);
        world->boat->hurt-=1;
    }

    if (world->boat->current_NPC != NULL){
            Vector3 new_pos = 
            Vector3(world->boat->pos.x,
                    world->boat->pos.y,
                    world->boat->pos.z);
            world->boat->current_NPC->pos = new_pos;
    }
    
    std::vector<NPC*> all_npc = world->all_npc;
    for(int i=0; i<3;i++){
        //all_npc[i]->mesh->model.setScale(5, 5, 5);
        all_npc[i]->mesh->model.setTranslation(all_npc[i]->pos.x,all_npc[i]->pos.y,all_npc[i]->pos.z);
        all_npc[i]->mesh->model.scale(10, 10, 10);
    }
    world->boat->mesh->model.setTranslation(world->boat->pos.x,world->boat->pos.y,world->boat->pos.z);
    
    if (world->boat->moving.length()==0){
        world->boat->mesh->model.rotate(t,Vector3(0,1,0));
        if(world->boat->current_NPC)
            world->boat->current_NPC->mesh->model.rotate(t,Vector3(0,1,0));
    }
    else{
        world->boat->mesh->model.rotate(angles[world->boat->mov_ind]*(PI/4),Vector3(0,1,0));
        if(world->boat->current_NPC)
            world->boat->current_NPC->mesh->model.rotate(angles[world->boat->mov_ind]*(PI/4),Vector3(0,1,0));
    
    }
    if(world->boat->current_NPC) world->boat->current_NPC->mesh->model.scale(0.5, 0.5, 0.5);

    int end_conditions = world->check_end();
    if (end_conditions != 0 && world->boat->hurt<=0){
        Game::instance->camera->lookAt(Vector3(-11.4899, 2.53442, 3.4166),Vector3(-44.9116, -13.6219, -48.7266), Vector3(0.f,1.f,0.f));
        Game::instance->curr_stage = END_STAGE;
        EndStage* es = (EndStage*)(Game::instance->stages[END_STAGE]);
        es->setupMovingEndStage();
        es->winlose = end_conditions;
        if (end_conditions>0) Audio::Play("data/assets/Sound/win.wav");
        else if (end_conditions<0) Audio::Play("data/assets/Sound/lose.wav");
        Audio::Stop("data/assets/Sound/music.wav");
        //std::cout<<es->winlose<<" "<<end_conditions<<std::endl;
    } 
    //to navigate with the mouse fixed in the middle
    //if (*mouse_locked)
    //    Input::centerMouse();

    //if (Input::wasKeyPressed(SDL_SCANCODE_P)) Game::instance->curr_stage = PAUSE_STAGE;
    //if (Input::wasKeyPressed(SDL_SCANCODE_T)) world->boat->movesAlone = 0;
    if (Input::wasKeyPressed(SDL_SCANCODE_R)) {restart = true; Audio::PlayParallel("data/assets/Sound/restart.wav");}

    if(restart){world->setup_level(world->gamemap);}
    
}

/*void PauseStage::render(){

}

void PauseStage::update(float seconds_elapsed){
    if (Input::wasKeyPressed(SDL_SCANCODE_P)) Game::instance->curr_stage = PLAY_STAGE;
    if (Input::wasKeyPressed(SDL_SCANCODE_RETURN)){
        Game::instance->curr_stage = PLAY_STAGE;
        PlayStage* ps = (PlayStage*)(Game::instance->stages[PLAY_STAGE]);
        ps->current_level += 1; //TODO: PRONE TO CRASH WHEN GETTING TO THE END
        if (ps->current_level >= ps->levels.size()){
            Game::instance->camera->lookAt(Vector3(-11.4899, 2.53442, 3.4166),Vector3(-44.9116, -13.6219, -48.7266), Vector3(0.f,1.f,0.f));
            Game::instance->curr_stage = END_STAGE; 
            EndStage* es = (EndStage*)(Game::instance->stages[END_STAGE]);
            es->winlose = -1;
        }
        else{
            ps->world->setup_level(ps->levels[ps->current_level]);
        }
    } 
    if (Input::wasKeyPressed(SDL_SCANCODE_M)){
        Game::instance->camera->lookAt(Vector3(-11.4899, 2.53442, 3.4166),Vector3(-44.9116, -13.6219, -48.7266), Vector3(0.f,1.f,0.f));
        Game::instance->curr_stage = MENU_STAGE;
    } 
}*/



EndStage::EndStage(void){
    Camera* camera = Game::instance->camera;
    camera->lookAt(Vector3(-11.4899, 2.53442, 3.4166),Vector3(-44.9116, -13.6219, -48.7266), Vector3(0.f,1.f,0.f));
    //camera->setPerspective(70.f,Game::instance->window_width/(float)Game::instance->window_height,0.1f,10000.f);
    
    Shader* s_hover = Shader::Get("data/shaders/hover.vs", "data/shaders/illumination.fs");
    Shader* s_basic = Shader::Get("data/shaders/basic.vs", "data/shaders/illumination.fs");
    Shader* s_fill = Shader::Get("data/shaders/quad.vs", "data/shaders/flat.fs");
    Shader* s_wind = Shader::Get("data/shaders/wind.vs", "data/shaders/illumination.fs");
    Shader* s_hover2 = Shader::Get("data/shaders/hover.vs", "data/shaders/illumination_lost.fs");

    Mesh* mesh_island = Mesh::Get("data/assets/Island/terrain-mountain-range_1.obj");
    Mesh* mesh_boat = Mesh::Get("data/assets/Boat/boat.obj");
    Mesh* mesh_text_l = Mesh::Get("data/assets/Font/youlost.obj");
    Mesh* mesh_text_w = Mesh::Get("data/assets/Font/youwon.obj");
    Mesh* mesh_quad = Mesh::getQuad();
    
    Mesh* mesh_flower1 = Mesh::Get("data/assets/Flowers/carnations_black.obj");
    Mesh* mesh_flower2 = Mesh::Get("data/assets/Flowers/roses_black.obj");
    Mesh* mesh_flower3 = Mesh::Get("data/assets/Flowers/carnations_red.obj");
    Mesh* mesh_flower4 = Mesh::Get("data/assets/Flowers/roses_red.obj");
    Mesh* mesh_flower5 = Mesh::Get("data/assets/Flowers/carnations_lightblue.obj");
    Mesh* mesh_flower6 = Mesh::Get("data/assets/Flowers/carnations_pink.obj");
    Mesh* mesh_flower7 = Mesh::Get("data/assets/Flowers/carnations_yellow.obj");
    Mesh* mesh_flower8 = Mesh::Get("data/assets/Flowers/roses_blue.obj");
    Mesh* mesh_flower9 = Mesh::Get("data/assets/Flowers/roses_orange.obj");
    Mesh* mesh_flower10 = Mesh::Get("data/assets/Flowers/roses_purple.obj");
    Mesh* mesh_flower11 = Mesh::Get("data/assets/Flowers/roses_yellow.obj");
    
    Mesh* mesh_play = Mesh::Get("data/assets/Font/play.obj");
    Mesh* mesh_quit = Mesh::Get("data/assets/Font/quit.obj");
   
    Texture* tex = new Texture();
    tex->load("data/assets/color-atlas-new.tga");
        
    island = new EntityMesh(mesh_island, tex, s_basic, Vector4(1,1,1,1));
    island->model.translate(-22.3277, 0.3, -8.81457);
    island->model.scale(0.5, 0.5, 0.5);
    island->model.rotate(-1.46, Vector3(0, 1, 0));
    
    boat = new EntityMesh(mesh_boat, tex, s_hover, Vector4(1,1,1,1));
    text_lost = new EntityMesh(mesh_text_l, tex, s_hover2, Vector4(1,1,1,1));
    this->setupMovingEndStage();

    text_won = new EntityMesh(mesh_text_w, tex, s_hover2, Vector4(1,1,1,1));
    text_won->model.setTranslation(-13.0475, 2.31, 1.22761);
    text_won->model.rotate(-0.53, Vector3(0, 1, 0));

    play_button = new EntityMesh(mesh_play, tex, s_hover2, Vector4(1,1,1,1));
    play_button->model.translate(-14.386, 0.880001, -1.98211);
    play_button->model.rotate(-0.47, Vector3(0, 1, 0));
    
    quit_button = new EntityMesh(mesh_quit, tex, s_basic, Vector4(0.3,0.3,0.3,1));
    quit_button->model.translate(-14.7159, 0.130002, -1.81454);
    quit_button->model.rotate(-0.47, Vector3(0, 1, 0));
    
    //selected = PLAY_BUTTON;
    
    quad = new EntityMesh(mesh_quad, tex, s_fill, Vector4(0.0, 0.0, 0.0, 0.5));
    
    //sink = true;
    
    pushLostFlowers(mesh_flower1, mesh_flower2, tex, s_wind);
    pushWinFlowers(mesh_flower3, mesh_flower4, mesh_flower5, mesh_flower6, mesh_flower7, mesh_flower8, mesh_flower9, mesh_flower10, mesh_flower11, tex, s_wind);
}

void EndStage::setupMovingEndStage(){
    boat->model.setTranslation(-16.6868, 0.3, -25.7556);
    boat->model.rotate(11.7402, Vector3(0, 1, 0));

    text_lost->model.setTranslation(-14.0723, 2.31, -0.165428);
    text_lost->model.rotate(-0.53, Vector3(0, 1, 0));
    
    //text_won->model.setTranslation(-13.0475, 2.31, 1.22761);
    //text_won->model.rotate(-0.53, Vector3(0, 1, 0));

    selected = PLAY_BUTTON;
    sink = true;
}

void EndStage::render(){

    if (winlose < 0){renderLost();}
    else renderWon();
}

void EndStage::renderLost()
{
    Camera* camera = Game::instance->camera;
    
    Game::instance->world->sky->model.setTranslation(camera->eye.x,camera->eye.y,camera->eye.z);
    Game::instance->world->sky->render();
    Game::instance->world->renderWater();

    island->render();
    boat->render();
    
    for(int i = 0; i < lostFlowers.size(); i++){
        lostFlowers[i]->render();
    }
    
    glDisable(GL_DEPTH_TEST);
    
    play_button->render();
    quit_button->render();
    
    glEnable(GL_BLEND);

    quad->render();
    
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    
    text_lost->render();
}

void EndStage::renderWon()
{
    Camera* camera = Game::instance->camera;
    
    Game::instance->world->sky->model.setTranslation(camera->eye.x,camera->eye.y,camera->eye.z);
    Game::instance->world->sky->render();
    Game::instance->world->renderWater();

    island->render();
    boat->render();
    
    for(int i = 0; i < winFlowers.size(); i++){
        winFlowers[i]->render();
    }
    
    glDisable(GL_DEPTH_TEST);
    
    play_button->render();
    quit_button->render();
    
    glEnable(GL_DEPTH_TEST);
    
    text_won->render();
}

void EndStage::update(float elapsed_time)
{
    Game::instance->camera->lookAt(Vector3(-11.4899, 2.53442, 3.4166),Vector3(-44.9116, -13.6219, -48.7266), Vector3(0.f,1.f,0.f));

    if (winlose < 0){updateLost();}
    else updateWon();
    
    //sceneMaker(text_won, 0.01);
        
}

void EndStage::updateLost(){
    //std::cout << "LOST" << std::endl;
    boat->model.rotate(sin(Game::instance->time)*0.002, Vector3(0, 0, 1));
    
    if(sink) boat->model.translate(0.0, -0.01, 0.0);
    if(boat->model.getTranslation().y <= -6.5) sink = false;
    
    // move text up to a certain point
    if(text_lost->model.getTranslation().x < -13.2282 && text_lost->model.getTranslation().z > -1.27547)
        text_lost->model.translate(0.0f, 0.0f, 0.005f);
    
    selectButton();
}

void EndStage::updateWon()
{
    selectButton();
}

void EndStage::selectButton()
{
    // SELECT PLAY BUTTON
    if (Input::wasKeyPressed(SDL_SCANCODE_UP) && selected == QUIT_BUTTON){
        selected = PLAY_BUTTON;
        
        play_button->shader = Shader::Get("data/shaders/hover.vs", "data/shaders/illumination_lost.fs");
        quit_button->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/illumination_lost.fs");
        
        play_button->color = Vector4(1,1,1,1);
        quit_button->color = Vector4(0.3, 0.3, 0.3, 1);

        Audio::Play("data/assets/Sound/menu_nav.wav");
    }
    
    // SELECT QUIT BUTTON
    if (Input::wasKeyPressed(SDL_SCANCODE_DOWN) && selected == PLAY_BUTTON){
        selected = QUIT_BUTTON;
        
        quit_button->shader = Shader::Get("data/shaders/hover.vs", "data/shaders/illumination_lost.fs");
        play_button->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/illumination_lost.fs");
        
        quit_button->color = Vector4(1,1,1,1);
        play_button->color = Vector4(0.3, 0.3, 0.3, 1);

        Audio::Play("data/assets/Sound/menu_nav.wav");
    }
    
    // PRESS SELECTED BUTTON
    if (Input::wasKeyPressed(SDL_SCANCODE_RETURN)){
        Audio::Play("data/assets/Sound/menu_ok.wav");
        if(selected == QUIT_BUTTON) Game::instance->must_exit = true;
        else{
            Game::instance->curr_stage = PLAY_STAGE;
            PlayStage* ps = (PlayStage*)(Game::instance->stages[PLAY_STAGE]);
            if(ps->current_level!=ps->levels.size()-1 || winlose<0){
                if (winlose>0) ps->current_level+=1;
                ps->world->setup_level(ps->levels[ps->current_level]);
                Audio::Play("data/assets/Sound/music.wav", 4);
            }
            else{
                Game::instance->camera->lookAt(Vector3(-11.4899, 2.53442, 3.4166),Vector3(-44.9116, -13.6219, -48.7266), Game::instance->camera->up);
                Game::instance->curr_stage = CREDITS_STAGE;
                CreditsStage* cs = (CreditsStage*)(Game::instance->stages[CREDITS_STAGE]);
                cs->setupMovingCredits();
            }
        }
    }
}

void EndStage::pushLostFlowers(Mesh* m1, Mesh* m2, Texture* tex, Shader* s)
{
    lostFlowers.push_back( new EntityMesh(m1, tex, s, Vector4(1,1,1,1)));
    lostFlowers[0]->model.translate(-16.1, 0.5, 0.2);
    
    lostFlowers.push_back( new EntityMesh(m2, tex, s, Vector4(1,1,1,1)));
    lostFlowers[1]->model.translate(-28.4, 7.1, -9.7);
    
    lostFlowers.push_back( new EntityMesh(m1, tex, s, Vector4(1,1,1,1)));
    lostFlowers[2]->model.translate(-19.9, 0.5, -0.2);
    lostFlowers[2]->model.rotate(-1.15, Vector3(0, 1, 0));
    
    lostFlowers.push_back( new EntityMesh(m1, tex, s, Vector4(1,1,1,1)));
    lostFlowers[3]->model.translate(-11, 0.5, -4);
    
    lostFlowers.push_back( new EntityMesh(m1, tex, s, Vector4(1,1,1,1)));
    lostFlowers[4]->model.translate(-15.4702, 0.5, 2.24205);
    lostFlowers[4]->model.rotate(-3.16, Vector3(0, 1, 0));
    
    lostFlowers.push_back( new EntityMesh(m2, tex, s, Vector4(1,1,1,1)));
    lostFlowers[5]->model.translate(-11.7702, 0.5, 1.54205);
    
    lostFlowers.push_back( new EntityMesh(m2, tex, s, Vector4(1,1,1,1)));
    lostFlowers[6]->model.translate(-15.8286, 0.5, -4.32644);
    lostFlowers[6]->model.rotate(-1.41, Vector3(0, 1, 0));
    
    lostFlowers.push_back( new EntityMesh(m2, tex, s, Vector4(1,1,1,1)));
    lostFlowers[7]->model.translate(-14.0718, 0.5, -9.40225);
    lostFlowers[7]->model.rotate(0.67, Vector3(0, 1, 0));
    
    lostFlowers.push_back( new EntityMesh(m2, tex, s, Vector4(1,1,1,1)));
    lostFlowers[8]->model.translate(-24.7064, 4, -13.8823);
    lostFlowers[8]->model.rotate(0.06, Vector3(0, 1, 0));
    
    lostFlowers.push_back( new EntityMesh(m1, tex, s, Vector4(1,1,1,1)));
    lostFlowers[9]->model.translate(-29.3001, 5.98794, -4.61804);
    lostFlowers[9]->model.rotate(-0.46, Vector3(1, 0, 0));
    
    lostFlowers.push_back( new EntityMesh(m2, tex, s, Vector4(1,1,1,1)));
    lostFlowers[10]->model.translate(-24.8702, 1.7, -4.95795);
}

void EndStage::pushWinFlowers(Mesh* m1, Mesh* m2, Mesh* m3, Mesh* m4, Mesh* m5, Mesh* m6, Mesh* m7, Mesh* m8, Mesh* m9, Texture* tex, Shader* s)
{
    winFlowers.push_back( new EntityMesh(m1, tex, s, Vector4(1,1,1,1)));
    winFlowers[0]->model.translate(-16.1, 0.5, 0.2);
    
    winFlowers.push_back( new EntityMesh(m2, tex, s, Vector4(1,1,1,1)));
    winFlowers[1]->model.translate(-28.4, 7.1, -9.7);
    
    winFlowers.push_back( new EntityMesh(m3, tex, s, Vector4(1,1,1,1)));
    winFlowers[2]->model.translate(-19.9, 0.5, -0.2);
    winFlowers[2]->model.rotate(-1.15, Vector3(0, 1, 0));
    
    winFlowers.push_back( new EntityMesh(m4, tex, s, Vector4(1,1,1,1)));
    winFlowers[3]->model.translate(-11, 0.5, -4);
    
    winFlowers.push_back( new EntityMesh(m5, tex, s, Vector4(1,1,1,1)));
    winFlowers[4]->model.translate(-15.4702, 0.5, 2.24205);
    winFlowers[4]->model.rotate(-3.16, Vector3(0, 1, 0));
    
    winFlowers.push_back( new EntityMesh(m6, tex, s, Vector4(1,1,1,1)));
    winFlowers[5]->model.translate(-11.7702, 0.5, 1.54205);
    
    winFlowers.push_back( new EntityMesh(m7, tex, s, Vector4(1,1,1,1)));
    winFlowers[6]->model.translate(-15.8286, 0.5, -4.32644);
    winFlowers[6]->model.rotate(-1.41, Vector3(0, 1, 0));
    
    winFlowers.push_back( new EntityMesh(m8, tex, s, Vector4(1,1,1,1)));
    winFlowers[7]->model.translate(-14.0718, 0.5, -9.40225);
    winFlowers[7]->model.rotate(0.67, Vector3(0, 1, 0));
    
    winFlowers.push_back( new EntityMesh(m9, tex, s, Vector4(1,1,1,1)));
    winFlowers[8]->model.translate(-24.7064, 4, -13.8823);
    winFlowers[8]->model.rotate(0.06, Vector3(0, 1, 0));
    
    winFlowers.push_back( new EntityMesh(m4, tex, s, Vector4(1,1,1,1)));
    winFlowers[9]->model.translate(-29.3001, 5.98794, -4.61804);
    winFlowers[9]->model.rotate(-0.46, Vector3(1, 0, 0));
    
    winFlowers.push_back( new EntityMesh(m6, tex, s, Vector4(1,1,1,1)));
    winFlowers[10]->model.translate(-24.8702, 1.7, -4.95795);
}

CreditsStage::CreditsStage(void){
    Camera* camera = Game::instance->camera;
    camera->lookAt(Vector3(-11.4899, 2.53442, 3.4166),Vector3(-44.9116, -13.6219, -48.7266), Vector3(0.f,1.f,0.f));
    //camera->setPerspective(70.f,Game::instance->window_width/(float)Game::instance->window_height,0.1f,10000.f);
    
    Shader* s_hover = Shader::Get("data/shaders/hover.vs", "data/shaders/illumination.fs");
    Shader* s_basic = Shader::Get("data/shaders/basic.vs", "data/shaders/illumination.fs");
    Shader* s_wind = Shader::Get("data/shaders/wind.vs", "data/shaders/illumination.fs");
    
    Mesh* mesh_text = Mesh::Get("data/assets/Font/credits.obj");
    Mesh* mesh_island = Mesh::Get("data/assets/Island/terrain-mountain-range_1.obj");
    Mesh* mesh_boat = Mesh::Get("data/assets/Boat/boat.obj");
    Mesh* mesh_flower1 = Mesh::Get("data/assets/Flowers/carnations_red.obj");
    Mesh* mesh_flower2 = Mesh::Get("data/assets/Flowers/roses_red.obj");
    Mesh* mesh_flower3 = Mesh::Get("data/assets/Flowers/carnations_lightblue.obj");
    Mesh* mesh_flower4 = Mesh::Get("data/assets/Flowers/carnations_pink.obj");
    Mesh* mesh_flower5 = Mesh::Get("data/assets/Flowers/carnations_yellow.obj");
    Mesh* mesh_flower6 = Mesh::Get("data/assets/Flowers/roses_blue.obj");
    Mesh* mesh_flower7 = Mesh::Get("data/assets/Flowers/roses_orange.obj");
    Mesh* mesh_flower8 = Mesh::Get("data/assets/Flowers/roses_purple.obj");
    Mesh* mesh_flower9 = Mesh::Get("data/assets/Flowers/roses_yellow.obj");
    
   
    Texture* tex = new Texture();
    tex->load("data/assets/color-atlas-new.tga");
    
    
    island = new EntityMesh(mesh_island, tex, s_basic, Vector4(1,1,1,1));
    island->model.translate(-22.3277, 0.3, -8.81457);
    island->model.scale(0.5, 0.5, 0.5);
    island->model.rotate(-1.46, Vector3(0, 1, 0));
    
    boat = new EntityMesh(mesh_boat, tex, s_hover, Vector4(1,1,1,1));
    boat->model.translate(-16.6868, 0.3, -25.7556);
    boat->model.rotate(11.7402, Vector3(0, 1, 0));
    
    text = new EntityMesh(mesh_text, tex, s_basic, Vector4(1,1,1,1));
    setupMovingCredits();
    
    flowers.push_back( new EntityMesh(mesh_flower1, tex, s_wind, Vector4(1,1,1,1)));
    flowers[0]->model.translate(-16.1, 0.5, 0.2);
    
    flowers.push_back( new EntityMesh(mesh_flower2, tex, s_wind, Vector4(1,1,1,1)));
    flowers[1]->model.translate(-28.4, 7.1, -9.7);
    
    flowers.push_back( new EntityMesh(mesh_flower3, tex, s_wind, Vector4(1,1,1,1)));
    flowers[2]->model.translate(-19.9, 0.5, -0.2);
    flowers[2]->model.rotate(-1.15, Vector3(0, 1, 0));
    
    flowers.push_back( new EntityMesh(mesh_flower4, tex, s_wind, Vector4(1,1,1,1)));
    flowers[3]->model.translate(-11, 0.5, -4);
    
    flowers.push_back( new EntityMesh(mesh_flower5, tex, s_wind, Vector4(1,1,1,1)));
    flowers[4]->model.translate(-15.4702, 0.5, 2.24205);
    flowers[4]->model.rotate(-3.16, Vector3(0, 1, 0));
    
    flowers.push_back( new EntityMesh(mesh_flower6, tex, s_wind, Vector4(1,1,1,1)));
    flowers[5]->model.translate(-11.7702, 0.5, 1.54205);
    
    flowers.push_back( new EntityMesh(mesh_flower7, tex, s_wind, Vector4(1,1,1,1)));
    flowers[6]->model.translate(-15.8286, 0.5, -4.32644);
    flowers[6]->model.rotate(-1.41, Vector3(0, 1, 0));
    
    flowers.push_back( new EntityMesh(mesh_flower8, tex, s_wind, Vector4(1,1,1,1)));
    flowers[7]->model.translate(-14.0718, 0.5, -9.40225);
    flowers[7]->model.rotate(0.67, Vector3(0, 1, 0));
    
    flowers.push_back( new EntityMesh(mesh_flower9, tex, s_wind, Vector4(1,1,1,1)));
    flowers[8]->model.translate(-24.7064, 4, -13.8823);
    flowers[8]->model.rotate(0.06, Vector3(0, 1, 0));
    
    flowers.push_back( new EntityMesh(mesh_flower4, tex, s_wind, Vector4(1,1,1,1)));
    flowers[9]->model.translate(-29.3001, 5.98794, -4.61804);
    flowers[9]->model.rotate(-0.46, Vector3(1, 0, 0));
    
    flowers.push_back( new EntityMesh(mesh_flower6, tex, s_wind, Vector4(1,1,1,1)));
    flowers[10]->model.translate(-24.8702, 1.7, -4.95795);
    
}

void CreditsStage::render()
{
    Camera* camera = Game::instance->camera;
    
    Game::instance->world->sky->model.setTranslation(camera->eye.x,camera->eye.y,camera->eye.z);
    Game::instance->world->sky->render();
    Game::instance->world->renderWater();
    
    island->render();
    boat->render();
    text->render();
    
    for(int i = 0; i < flowers.size(); i++){
        flowers[i]->render();
    }
    
}

void CreditsStage::update(float elapsed_time)
{
    if(text->model.getTranslation().y < 6.1) text->model.translate(0.0, 0.01, 0.0);
    else if (!reproduced){
        reproduced=true; 
        Audio::Play("data/assets/Sound/end.wav");
    }
    if (Input::wasKeyPressed(SDL_SCANCODE_M)){
        Game::instance->curr_stage = MENU_STAGE;
        Audio::Play("data/assets/Sound/menu_ok.wav");
    }
}

void CreditsStage::setupMovingCredits(){
    text->model.setTranslation(-14.6241, -4.47001, -1.43107);
    text->model.rotate(-0.53, Vector3(0, 1, 0));
    reproduced = false;
}