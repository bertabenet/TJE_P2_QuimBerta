#include "game.h"
#include "mygame.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"

#include <cmath>

//some globals
//Mesh* island_mesh = NULL;
//Texture* texture = NULL;
//Mesh* mesh_boat = NULL;
//Mesh* mesh_island = NULL;
//Texture* texture2 = NULL;
Mesh* sky_mesh = NULL;
Texture* sky_tex = NULL;
//Shader* shader = NULL;
//Animation* anim = NULL;
//float angle = 0;
FBO* fbo = NULL;

Game* Game::instance = NULL;
//World* World::instance = NULL;

//Player* player;

Game::Game(int window_width, int window_height, SDL_Window* window)
{
    this->window_width = window_width;
    this->window_height = window_height;
    this->window = window;
    instance = this;
    must_exit = false;

    fps = 0;
    frame = 0;
    time = 0.0f;
    elapsed_time = 0.0f;
    mouse_locked = false;

    //OpenGL flags
    glEnable( GL_CULL_FACE ); //render both sides of every triangle
    glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

    //create our camera
    camera = new Camera();
    camera->lookAt(Vector3(0.f,100.f, 100.f),Vector3(0.f,0.f,0.f), Vector3(0.f,1.f,0.f)); //position the camera and point to 0,0,0
    camera->setPerspective(70.f,window_width/(float)window_height,0.1f,10000.f); //set the projection, we want to be perspective

    initWorld();

    //easier, provisional alternative to test PlayStages
    //Mesh* island_Mesh = Mesh::Get("data/assets/Low Poly Pirate Landscapes/Low Poly Pirate Landscapes.obj");

}

void Game::initWorld(){
    
    Texture* texture = new Texture();
    texture->load("data/assets/color-atlas-new.tga");
    Mesh* mesh_boat = Mesh::Get("data/assets/Boat/raft-flag_5.obj");
    Mesh* mesh_island = Mesh::Get("data/assets/Island/terrain-mountain-range_1.obj");
    Mesh* mesh_penguin = Mesh::Get("data/assets/NPCs/penguin_20.obj");
    
    //mesh_islands = Mesh::Get("data/assets/Low Poly Pirate Landscapes/Low Poly Pirate Landscapes.obj");
    
    Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/illumination.fs");

    SDL_ShowCursor(!mouse_locked); //hide or show the mouse

    world = new World();

    // PLAYER
    EntityMesh* boat = new EntityMesh(mesh_boat, texture, shader, Vector4(1, 1, 1, 1), Vector3(50, 0, 0));
    world->boat = new Player(Vector3(0, 0, 0), eDirection::LEFT, nullptr, boat, nullptr);
    world->boat->mesh->model.scale(10, 10, 10);
    
    //NPC
    EntityMesh* penguin_m = new EntityMesh(mesh_penguin, texture, shader, Vector4(1, 1, 1, 1), Vector3(1, 1, 1));
    NPC* penguin = new NPC(Vector3(0, 0, 0), SHEEP, penguin_m);
    world->all_npc.push_back(penguin);
    
    // ISLAND
    EntityMesh* islandm_01 = new EntityMesh(mesh_island, texture, shader, Vector4(1, 1, 1, 1), Vector3(10, 10, 10));
    islandm_01->model.scale(0.5, 0.5, 0.5);
    Island* island_01 = new Island( Vector3(10,10,0), NORMAL, islandm_01);
    island_01->addNPC(penguin);
    
    
    //PlayStage proof of concept
    curr_stage = PLAY_STAGE; //test
    
    stages[PAUSE_STAGE] = new PauseStage();
    stages[PLAY_STAGE] = new PlayStage();

    PlayStage* ps = (PlayStage*) stages[PLAY_STAGE];
    ps->addLevel(new Level());
        
    ps->levels[0]->addIsland(island_01);
    
    world->islands = &(ps->levels[0]->islands);
    
}

//what to do when the image has to be draw
void Game::render(void)
{
    //set the clear color (the background color)
    glClearColor(0.0, 0.0, 0.0, 1.0);

    // Clear the window and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //set the camera as default
    camera->enable();

    //set flags
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
   
    stages[curr_stage]->render();

    //Draw the floor grid
    drawGrid();

    //render the FPS, Draw Calls, etc
    drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

    //swap between front buffer and back buffer
    SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{
    stages[curr_stage]->update(seconds_elapsed);
}

//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{
    switch(event.keysym.sym)
    {
        case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
        case SDLK_F1: Shader::ReloadAll(); break;
    }
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{
}

void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Game::onMouseButtonDown( SDL_MouseButtonEvent event )
{
    if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
    {
        mouse_locked = !mouse_locked;
        SDL_ShowCursor(!mouse_locked);
    }
}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Game::onMouseWheel(SDL_MouseWheelEvent event)
{
    mouse_speed *= event.y > 0 ? 1.1 : 0.9;
}

void Game::onResize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
    glViewport( 0,0, width, height );
    camera->aspect =  width / (float)height;
    window_width = width;
    window_height = height;
}

