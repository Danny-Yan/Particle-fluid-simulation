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

#include "LTexture.h"
#include "Dot.h"
#include "collision_check.h"
#include "constants.h"
#include "time.h"
#include "helper_main_functions.h"
#include "helper_structs.h"

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

    //Modulation components
    Uint8 r = 255;
    Uint8 g = 255;
    Uint8 b = 255;
    Uint8 colour = 255;

    // Spacing variables
    float timeInterval = 1.0f; // Doesnt work
    float spacing = 10;
    int radius = 10;           // Doesnt work

    // Spacing equations
    float spacing_scale = radius * HITBOX_SCALE + spacing;
    int particlesPerRow = (int)sqrt(PARTICLE_NUM);
    int particlesPerCol = (PARTICLE_NUM - 1) / particlesPerRow + 1;
    float x_cord;
    float y_cord;

    // Initialising vectors
    std::vector<Entry> particleHashEntries(PARTICLE_NUM);
    std::vector<int> spacialKeys(PARTICLE_NUM);
    std::vector<Dot> dots;

    for ( int i = 0; i < PARTICLE_NUM; i++ )
    {
        // radius = std::rand() % 10 + 5;
        float x = (i % particlesPerRow - particlesPerRow / 2.0f + 0.5f) * (spacing_scale);
        float y = (i / particlesPerRow - particlesPerCol / 2.0f + 0.5f) * (spacing_scale);

        x_cord = x + 700;
        y_cord = y + 450;
        Dot dot( x_cord, y_cord, std::rand() % 10, std::rand() % 10, radius);
        dots.push_back( dot );
    }

    //Set the wall
    SDL_Rect wall;
    wall.x = 500;
    wall.y = 400;
    wall.w = 0;
    wall.h = 0;

    //Timer
    Uint32 Ticks = 0;
    Uint32 deltaTime = 0;
    LTimer timer;
    timer.start();

    //Mouse
    Mouse mouse;
    int xPrevMouse = 0;
    int yPrevMouse = 0;

    //While application is running
    while( !quit )
    {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }
            
            if (e.type == SDL_MOUSEMOTION)
            {
                SDL_GetMouseState( &mouse.x, &mouse.y );
            }

            //Reset start time on return keypress
            else if( e.type == SDL_KEYDOWN )
            {
                //Start/stop
                if( e.key.keysym.sym == SDLK_s )
                {
                    if( timer.isStarted() )
                    {
                        timer.stop();
                    }
                    else
                    {
                        timer.start();
                    }
                }
                //Pause/Unpause
                else if( e.key.keysym.sym == SDLK_p )
                {
                    if( timer.isPaused() )
                    {
                        timer.resetTicked();
                        timer.unpause();
                    }
                    else
                    {
                        timer.pause();
                    }
                }
                else if( e.key.keysym.sym == SDLK_t )
                {
                    if( timer.isPaused() )
                    {
                        timer.unpause();
                        timer.setTicked();
                    }
                } 
            }
        }

        //Clear screen
        SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
        SDL_RenderClear( gRenderer );

        //Render wall
        SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );        
        SDL_RenderDrawRect( gRenderer, &wall );

        deltaTime += timer.getTicks(); // After pausing for an extended period of time then unpausing, the entire system freezes
        if ( (float)deltaTime >= timeInterval && timer.isPaused() == 0 )
        {
            if (timer.getTicked() == 1){
                timer.pause();
            }
            mouse.velX = (mouse.x - xPrevMouse) / timeInterval;
            mouse.velY = (mouse.y - yPrevMouse) / timeInterval;
            xPrevMouse = mouse.x;
            yPrevMouse = mouse.y;

            //Move all dots
            for (Dot &dot : dots)
            {
                dot.moveVector( timeInterval );
            }

            // Update spacial lookup after moving 
            updateSpacialLookup( particleHashEntries, spacialKeys, dots );

            // Check collision for all dots
            for ( int i = 0; i < PARTICLE_NUM; i++)
            {
                Dot &dot = dots[i];
                dot.check_vector_collision( mouse, timeInterval, wall, dots, particleHashEntries, spacialKeys, i );

                int speed = abs(dot.getVelX()) + abs(dot.getVelY());
                Uint8 colour = 255 - std::min(speed * 10, 255);
                // r = std::rand() % 255;
                // g = std::rand() % 255;
                // b = std::rand() % 255;
                r = colour;
                gDotTexture.setColor(r, g, b);
                dot.render( gRenderer, gDotTexture );
            }

            //Update screen
            SDL_RenderPresent( gRenderer );
            deltaTime = 0;
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}
