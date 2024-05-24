#ifndef DOT_H
#define DOT_H

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdio.h>
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

        //Checks collision with another dot or wall
        void check_collision( float deltaTime, SDL_Rect& wall, std::vector<Dot>& circles, std::vector<Entry>& particleHashEntries, std::vector<int> &spacialKeys, int index);

        //Shows the dot on the screen
        void render( SDL_Renderer *gRenderer, LTexture& gDotTexture);

        //Getters
        Circle& getColliders();
        float getVelX();
        float getVelY();
        float getSpacialX();
        float getSpacialY();

        void compute_spacial_coords();

        //Computes the hash value of a dot's spacial coords
        int compute_spacial_hash(int spacialX, int spacialY);

    private:
        //The X and Y offsets of the dot
        int PosX, PosY;
        int mPosX, mPosY;

        //Spacial X and Y offsets of the dot
        int spacialX, spacialY;
        int spacial_hash;

        //The velocity of the dot
        int mVelX, mVelY;
        int Velx, Vely;
        
        //Dot's collision circle
        Circle mCollider;

        //Dot's collision vector with another dot
        Collision cLVector;

        //Moves the collision circle relative to the dot's offset
        void shiftColliders();

        Collision checkDotCollision( Dot& b );
};


#endif