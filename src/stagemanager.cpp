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
    
    if (Input::isKeyPressed(SDL_SCANCODE_UP)) player->mesh->model.translate(0.0f, 0.0f, -0.1f);
    if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) player->mesh->model.translate(0.0f, 0.0f, 0.1f);
    if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) player->mesh->model.translate(-0.1f, 0.0f, 0.0f);
    if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) player->mesh->model.translate(0.1f,0.0f, 0.0f);

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