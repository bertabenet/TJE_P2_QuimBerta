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

    fbo = new FBO();
    //fbo->create(window_width, window_height);

    //OpenGL flags
    glEnable( GL_CULL_FACE ); //render both sides of every triangle
    glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

    //create our camera
    camera = new Camera();
    //camera->lookAt(Vector3(0.f, 5.f, 50.f),Vector3(0.f,0.f,0.f), Vector3(0.f,1.f,0.f)); //position the camera and point to 0,0,0
    //camera->setPerspective(70.f,window_width/(float)window_height,0.1f,10000.f); //set the projection, we want to be perspective
    camera->lookAt(Vector3(25.f, 25.f, 50.f),Vector3(0.f,0.f,20.f), Vector3(0.f,1.f,0.f)); 
    camera->setPerspective(70.f,Game::instance->window_width/(float)Game::instance->window_height,0.1f,10000.f); 

    initWorld();

    //easier, provisional alternative to test PlayStages
    //Mesh* island_Mesh = Mesh::Get("data/assets/Low Poly Pirate Landscapes/Low Poly Pirate Landscapes.obj");

}

void Game::initWorld(){

    world = new World();

    texture_atlas = new Texture();
    texture_atlas->load("data/assets/color-atlas-new.tga");
    mesh_boat = Mesh::Get("data/assets/Boat/boat.obj");
    mesh_island = Mesh::Get("data/assets/Island/terrain-mountain-range_1.obj");
    mesh_bear = Mesh::Get("data/assets/NPCs/bear_brown_6.obj");   // wolf
    mesh_penguin = Mesh::Get("data/assets/NPCs/penguin.obj");  // sheep
    mesh_rat = Mesh::Get("data/assets/NPCs/rat.obj");          // cabbage
    
    Mesh* sky_mesh = Mesh::Get("data/assets/cielo/cielo.ASE");
    Texture* sky_tex = new Texture();
    sky_tex->load("data/assets/cielo/cielo.tga");
    world->sky = new EntityMesh(sky_mesh, sky_tex, Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs"), Vector4(1,1,1,1));

    Texture* sea_tex = new Texture();
    sea_tex->load("data/assets/Water/water.tga");

    Mesh* sea_mesh = new Mesh();
    sea_mesh->createPlane(200.0f);
    world->sea = new EntityMesh(sea_mesh, sea_tex, Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs"), Vector4(1,1,1,1));
    world->sea->tiling = 100.0f;
    world->sea->t_coef = 0.05f;
    
    Mesh* seapath_mesh = new Mesh();
    seapath_mesh->createPlane(world->tile_offset/2);
    world->seapath = new EntityMesh(seapath_mesh, sea_tex, Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs"), Vector4(0.8,0.8,1,1));
    world->seapath->tiling = 3.0f;
    world->seapath->t_coef = 0.05f;

    SDL_ShowCursor(!mouse_locked); //hide or show the mouse

    //PlayStage proof of concept
    curr_stage = MENU_STAGE; //test
    
    stages[MENU_STAGE] = new MenuStage();
    stages[PAUSE_STAGE] = new PauseStage();
    stages[PLAY_STAGE] = new PlayStage();

    PlayStage* ps = (PlayStage*) stages[PLAY_STAGE];
    ps->addLevel(loadGameMap("data/assets/Tiles/mymap1.map"));
    
    world->setup_level(ps->levels[ps->current_level]);
}

//what to do when the image has to be draw
void Game::render(void)
{
    //glClearColor(0.0, 0.0, 0.0, 1.0);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //fbo->bind();//TODO: CLICK!
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

    //fbo->color_textures[0]->toViewport(); //TODO: CLICK!
    //fbo->unbind();
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
    if (event.button == SDL_BUTTON_LEFT) //middle mouse
    {
        Input::clicked = true;
    }
    if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
    {
        mouse_locked = !mouse_locked;
        SDL_ShowCursor(!mouse_locked);
        //std::cout<< "iau" <<std::endl;
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

