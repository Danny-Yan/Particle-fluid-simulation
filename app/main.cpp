//Using SDL and standard IO
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

#include "incl/LTexture.h"
#include "incl/Dot.h"
#include "incl/collision_check.h"
#include "incl/constants.h"
#include "incl/time.h"
#include "incl/helper_main_functions.h"
#include "incl/helper_structs.h"

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gwindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture gDotTexture;

/*-------------------------------------------------------------------------------------*/

bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Create window
        gwindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gwindow == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer( gwindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if( gRenderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }
            }
        }
    }

    return success;
}

void close()
{
    //Free loaded images
    gDotTexture.free();

    //Destroy window    
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gwindow );
    gwindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load Foo' texture
    if( !gDotTexture.loadFromFile( gRenderer, "images/transparent_dot.png" ) )
    {
        printf( "Failed to load dot' texture image!\n" );
        success = false;
    }

    // else
    // {
    //     //Set standard alpha blending
    //     gDotTexture.setBlendMode( SDL_BLENDMODE_BLEND );
    // }
    return success;
}

/*-------------------------------------------------------------------------------------*/

int main( int argc, char* args[] )
{
    //Start up SDL and create window
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
        return 1;
    }

    //Load media
    if( !loadMedia() )
    {
        printf( "Failed to load media!\n" );
        return 1;
    }

    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;

    // Spacing equations
    float spacing_scale = RADIUS * SCALE + SPACING;
    int particlesPerRow = (int)sqrt(PARTICLE_NUM);
    int particlesPerCol = (PARTICLE_NUM - 1) / particlesPerRow + 1;
    float x_cord;
    float y_cord;
    float particle_density;

    // Initialising vectors
    std::vector<Entry> particleHashEntries(PARTICLE_NUM);
    std::vector<int> spacialKeys(PARTICLE_NUM);
    std::vector<Dot> dots;
    std::vector<Dot*> filtered_dots;

    for ( int i = 0; i < PARTICLE_NUM; i++ )
    {
        // radius = std::rand() % 10 + 5;
        float x = (i % particlesPerRow - particlesPerRow / 2.0f + 0.5f) * (spacing_scale);
        float y = (i / particlesPerRow - particlesPerCol / 2.0f + 0.5f) * (spacing_scale);

        x_cord = x + 700;
        y_cord = y + 450;
        Dot dot( x_cord, y_cord, 0, 0, RADIUS);
        dots.push_back( dot );
    }

    for ( int i = 0; i < PARTICLE_NUM; i++ )
    {
        Dot &dot = dots[i];
        // Filters dots only within the spatial vicinity

        std::vector<int> spacial_hashes_full = compute_full_spatial_area((int)dot.getsPosX(), (int)dot.getsPosY());

        particleFilter( filtered_dots, dots, particleHashEntries, spacialKeys, dot ); // filtered_dots

        // Calc density of a position (particle position)
        calculateDensity( particle_density, filtered_dots, dot.getPosX(), dot.getPosY() );
        dot.setDensity( particle_density );
    }

    //Set the wall
    SDL_Rect wall;
    wall.x = 500;
    wall.y = 400;
    wall.w = 0;
    wall.h = 0;

    //Timer
    Uint64 Ticks = 0;
    Uint64 deltaTime = 0;
    Uint64 interval = 0;
    LTimer timer;
    timer.start();

    //Mouse
    Mouse mouse( FORCE_RADIUS, MOUSE_FORCE_RADIUS );

    //While application is running
    while( !quit )
    {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            switch(e.type){
                // QUIT
                case SDL_QUIT:
                    quit = true;
                    break;

                // MOUSE MOVE
                case SDL_MOUSEMOTION:
                    SDL_GetMouseState( &mouse.x, &mouse.y );
                    mouse.move();
                    break;
                
                // MOUSE CLICK
                case SDL_MOUSEBUTTONDOWN:
                    switch (e.button.button){
                        case SDL_BUTTON_LEFT:
                            mouseLeftPress( e.button , &mouse);
                            break;
                        case SDL_BUTTON_RIGHT:
                            mouseRightPress(e.button , &mouse);
                            break;
        
                    }
                    break;

                case SDL_MOUSEBUTTONUP:
                    mouseUnPress( e.button , &mouse);
                    break;
        
                // KEYBOARD
                case SDL_KEYDOWN:
                    switch ( e.key.keysym.sym){

                        // Start/stop
                        case SDLK_s:
                            if( timer.isStarted() )
                            {
                                timer.stop();
                            }
                            else
                            {
                                timer.start();
                            }
                            break;
                        
                        // Pause/Unpause
                        case SDLK_p:
                            if( timer.isPaused() )
                            {
                                timer.resetTicked();
                                timer.unpause();
                            }
                            else
                            {
                                timer.pause();
                            }
                            break;

                        // Ticked
                        case SDLK_t:
                            if( timer.isPaused() )
                            {
                                timer.unpause();
                                timer.setTicked();
                            }
                            break;
                    }
            }
        }

        //Clear screen
        SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
        SDL_RenderClear( gRenderer );

        //Render wall
        SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );        
        SDL_RenderDrawRect( gRenderer, &wall );

        float deltaTime = (timer.getTicks() - interval) * 100000 / (float) SDL_GetPerformanceFrequency(); // Get the time passed in ms since the start of the frame.        
        // printf("%f\n", deltaTime);
        if ( deltaTime >= TIMEINTERVAL && timer.isPaused() == 0 )
        {

            // // CONTACT METHOD ------------------------------------------------------------------------------------------------------
            // for (Dot &dot : dots)
            // {
            //     dot.moveVector(TIMEINTERVAL);
            // }
            
            // // Update spacial lookup after moving
            // updateSpatialLookup( particleHashEntries, spacialKeys, dots );

            // // Check collision for all dots
            // for ( int i = 0; i < PARTICLE_NUM; i++)
            // {
            //     Dot &dot = dots[i];
            //     dot.check_vector_collision( TIMEINTERVAL, wall, dots, particleHashEntries, spacialKeys, i );

            //     int speed = abs(dot.getVelX()) + abs(dot.getVelY());
            //     Uint8 colour = 255 - std::min(speed * 10, 255);
            //     // r = std::rand() % 255;
            //     // g = std::rand() % 255;
            //     // b = std::rand() % 255;
            //     r = colour;
            //     gDotTexture.setColor(r, g, b);
            //     dot.render( gRenderer, gDotTexture );
            // }
            // // ========================================================================================================================
            
            // FORCE METHOD (Doesn't work) ---------------------------------------------------------------------------------------------------------
            // // Check force collision for all dots
            // for ( int i = 0; i < PARTICLE_NUM; i++)
            // {
            //     Dot &dot = dots[i];

            //     // Finds all dots within a vicinity
            //     particleFilter( filtered_dots, dots, particleHashEntries, spacialKeys,  dot.getmPosX(), dot.getmPosY() ); // filtered_dots
                
            //     // COLLISION
            //     for (Dot* dotB: filtered_dots ){
            //         dot.check_vector_force( *dotB );
            //     }
            //     dot.check_mouse_force( mouse );
            //     dot.check_wall_collision();

            //     // COLORING
            //     int speed = abs(dot.getVelX()) + abs(dot.getVelY());
            //     Uint8 colour = 255 - std::min(speed * 10, 255);
            //     // r = std::rand() % 255;
            //     // g = std::rand() % 255;
            //     // b = std::rand() % 255;
            //     r = colour;
            //     gDotTexture.setColor(r, g, b);

            //     // RENDERING
            //     dot.render( gRenderer, gDotTexture );
            // }


            // // //DENSITY METHOD -------------------------------------------------------------------------------------------------------

            // //Move all dots
            // for (Dot &dot : dots)
            // {
            //     dot.movePrediction( 5.0f );
            // }

            // // Update spacial lookup after moving
            // updateSpatialLookup( particleHashEntries, spacialKeys, dots );

            // //Update densities after moving
            // updateDensities( dots, particleHashEntries, spacialKeys );

            // for ( int i = 0; i < PARTICLE_NUM; i++ )
            // {
            //     Dot &dot = dots[i];
            //     dot.check_wall_no_shift();

            //     // Finds all dots within a vicinity
            //     particleFilter( filtered_dots, dots, particleHashEntries, spacialKeys, dot ); // filtered_dots

            //     std::vector<float> pressures = calculatePressureGradient( filtered_dots, &dot ); // Adding up the gradients for all dots within the vicinity

            //     if (abs(dot.getDensity()) > 0.001f)
            //     {
            //         dot.addmVelX(pressures[0] / dot.getDensity());
            //         dot.addmVelY(pressures[1] / dot.getDensity());
            //     }

            //     // COLORING
            //     int speed = abs( dot.getVelX() ) + abs( dot.getVelY() );
            //     Uint8 colour = 255 - std::min(speed * 10, 255);
            //     // r = std::rand() % 255;
            //     // g = std::rand() % 255;
            //     // b = std::rand() % 255;
            //     r = colour;
            //     gDotTexture.setColor(r, g, b);

            //     // RENDERING
            //     dot.render( gRenderer, gDotTexture );
            // }

            // // ========================================================================================================================

            //DENSITY METHOD w/ PREDICTIVE STEPS -------------------------------------------------------------------------------------------------------

            //Move all dots
            for (Dot &dot : dots)
            {
                dot.movePrediction( 1.0f, 0.5f );
            }

            // Update spacial lookup after moving
            updateSpatialLookup( particleHashEntries, spacialKeys, dots );

            //Update densities after moving
            updateDensities( dots, particleHashEntries, spacialKeys );

            for ( int i = 0; i < PARTICLE_NUM; i++ )
            {
                Dot &dot = dots[i];
                dot.check_wall_no_shift();
                
                std::vector<float> pressureGradient = { 0, 0 };

                // Finds all dots within a vicinity
                //Computing 3x3 spacial hash 
                std::vector<int> spacial_hashes_full = compute_full_spatial_area((int)dot.getsPosX(), (int)dot.getsPosY());

                //Iterating through spacial hashes
                for (int hash : spacial_hashes_full)
                {
                    int key = spacialKeys[hash];
                    if ( key == INT_MAX){
                        continue;
                    }
                    for (int i = key; i < dots.size(); i++)
                    {
                        if (particleHashEntries[i].hash != hash){
                            break;
                        }

                        Dot &dotB = dots[particleHashEntries[i].index];
                        if ( &dotB == &dot )
                        {
                            continue;
                        }
                        // dot.applyDotCollison( dotB );

                        // // Calc mouse force
                        // dot.check_mouse_force( &mouse );

                        // Calc pressure gradient (Loop and keep track of pressure gradient)
                        pressureGradient = calculatePressureGradient( pressureGradient, &dotB, &dot ); 
                    }
                }
                
                if (abs(dot.getDensity()) > DENSITY_UPPER)
                {
                    dot.addmVelX(pressureGradient[0] / dot.getDensity());
                    dot.addmVelY(pressureGradient[1] / dot.getDensity());
                }
            }

            //Move all dots
            for (Dot &dot : dots)
            {
                // COLORING
                int speed = (abs( dot.getVelX() ) + abs( dot.getVelY() )) * 100;
                std::vector<Uint8> colors = colourProcessor(speed);
                gDotTexture.setColor(colors[0], colors[1], colors[2]);

                // RENDERING
                dot.render( gRenderer, gDotTexture );
            }

            // ========================================================================================================================

            //Update screen
            SDL_RenderPresent( gRenderer );
            interval = timer.getTicks();
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}
