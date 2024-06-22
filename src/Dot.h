#ifndef DOT_H
#define DOT_H

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdio.h>
#include <cmath>
#include <algorithm>
#include "collision_check.h"
#include "LTexture.h"
#include "constants.h"
#include "helper_structs.h"
#include "spatial_hashers.h"
#include "collider.h"
#include "Mouse.h"

//The dot that will move around on the screen
class Dot : public Collider
{
    public:
        //Initializes the variables
        Dot( int x, int y , int velX, int velY, int radius);

        //Moves the dot
        void moveVector( float deltaTime );

        //Check collision with another dot
        void check_vector_collision( float deltaTime, SDL_Rect& square, std::vector<Dot>& circles, std::vector<Entry>& particleHashEntries, std::vector<int> &spacialKeys, int index );
        void check_vector_force( Dot &dot );
        void check_wall_collision();
        void check_mouse_force( Mouse &mouse );

        //Getters
        float getVelX();
        float getVelY();
        float getmPosX();
        float getmPosY();

        //Setters
        void setmVelX( float velX );
        void setmVelY( float velY );

        //Shows the dot on the screen
        void render( SDL_Renderer *gRenderer, LTexture& gDotTexture);

    private:
        //The X and Y offsets of the dot
        float mPosX, mPosY;
        float PosX, PosY;

        //The velocity of the dot
        float mVelX, mVelY;
        float Velx, Vely;

        //Dot's collision vector with another dot
        Collision cLVector;

        void shiftColliders() override;
        Collision checkDotCollision( Dot& b );
        Collision checkCircleForce( Collider& b );
};


#endif