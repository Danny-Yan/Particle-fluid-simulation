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

//The dot that will move around on the screen
class Dot
{
    public:
        //Initializes the variables
        Dot( int x, int y , int velX, int velY, int radius );

        //Update velocity
        void updateVelocity();

        //Moves the dot
        void move();
        void moveVector( float deltaTime );

        //Checks collision with another dot or wall
        void check_collision( float deltaTime, SDL_Rect& wall, std::vector<Dot>& circles, Mouse mouse, std::vector<Entry>& particleHashEntries, std::vector<int> &spacialKeys, int index);

        void check_vector_collision( float deltaTime, SDL_Rect& square, std::vector<Dot>& circles, std::vector<Entry>& particleHashEntries, std::vector<int> &spacialKeys, int index );

        //Shows the dot on the screen
        void render( SDL_Renderer *gRenderer, LTexture& gDotTexture);

        //Getters
        Circle& getColliders();
        float getVelX();
        float getVelY();
        int getSpacialX();
        int getSpacialY();
        void setmVelX( float velX );
        void setmVelY( float velY );

        void compute_spacial_coords();

        //Computes the hash value of a dot's spacial coords
        int compute_spacial_hash(int spacialX, int spacialY);

        void compute_spacial_area();
        std::vector<int> compute_full_spacial_area();

    private:
        //The X and Y offsets of the dot
        float PosX, PosY;
        float mPosX, mPosY;

        //Spacial X and Y offsets of the dot
        int spacialX, spacialY;
        int spacial_hash;
        std::vector<int> spacial_hashesX;
        std::vector<int> spacial_hashesY;
        std::vector<int> spacial_hashes_full;

        //The velocity of the dot
        float mVelX, mVelY;
        float Velx, Vely;
        
        //Dot's collision circle
        Circle mCollider;

        //Dot's collision vector with another dot
        Collision cLVector;

        //Moves the collision circle relative to the dot's offset
        void shiftColliders();

        Collision checkDotCollision( Dot& b );
        Collision checkDotCollision( Mouse mouse );
};


#endif