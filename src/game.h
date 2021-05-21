/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
    This class encapsulates the game, is in charge of creating the game, getting the user input, process the update and render.
*/

#ifndef GAME_H
#define GAME_H

#include "includes.h"
#include "camera.h"
#include "utils.h"
#include "mygame.h"
#include "stagemanager.h"

#define N_STAGES 6
#define MAX_N_PLAYSTAGES 5

class Game
{
public:
    static Game* instance;
    World* world;

    //window
    SDL_Window* window;
    int window_width;
    int window_height;

    //some globals
    long frame;
    float time;
    float elapsed_time;
    int fps;
    bool must_exit;

    //some vars
    Camera* camera; //our global camera
    bool mouse_locked; //tells if the mouse is locked (not seen)
    float mouse_speed = 10.0f;

    Game( int window_width, int window_height, SDL_Window* window );
    void initWorld();

    //main functions
    void render( void );
    void update( double dt );

    //events
    void onKeyDown( SDL_KeyboardEvent event );
    void onKeyUp(SDL_KeyboardEvent event);
    void onMouseButtonDown( SDL_MouseButtonEvent event );
    void onMouseButtonUp(SDL_MouseButtonEvent event);
    void onMouseWheel(SDL_MouseWheelEvent event);
    void onGamepadButtonDown(SDL_JoyButtonEvent event);
    void onGamepadButtonUp(SDL_JoyButtonEvent event);
    void onResize(int width, int height);

    Stage * stages[N_STAGES];
	eStage_types curr_stage = MENU_STAGE;

    TileMap* gamemap;
    EntityMesh* sea; //TODO: DELETE OR MOVE;
};


#endif 
