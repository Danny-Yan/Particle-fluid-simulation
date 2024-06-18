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

        //Moves the dot
        void moveVector( float deltaTime );

        //Check collision with another dot
        void check_vector_collision( Mouse mouse, float deltaTime, SDL_Rect& square, std::vector<Dot>& circles, std::vector<Entry>& particleHashEntries, std::vector<int> &spacialKeys, int index );

        //Shows the dot on the screen
        void render( SDL_Renderer *gRenderer, LTexture& gDotTexture);

        //Getters
        Circle& getColliders();
        float getVelX();
        float getVelY();
        int getSpacialX();
        int getSpacialY();

        //Setters
        void setmVelX( float velX );
        void setmVelY( float velY );

        //Computes the spacial coords of a position
        void compute_spacial_coords();

        //Computes the hash values of a dot's spacial coords
        int compute_spacial_hash(int spacialX, int spacialY);
        std::vector<int> compute_full_spacial_area();

    private:
        //The X and Y offsets of the dot
        float mPosX, mPosY;
        float PosX, PosY;

        //The velocity of the dot
        float mVelX, mVelY;
        float Velx, Vely;

        //Spacial X and Y offsets of the dot
        int spacialX, spacialY;
        int spacial_hash;
        std::vector<int> spacial_hashes_full;
        
        //Dot's collision circle
        Circle mCollider;

        //Dot's collision vector with another dot
        Collision cLVector;

        //Moves the collision circle relative to the dot's offset
        void shiftColliders();

        Collision checkDotCollision( Dot& b );
};


#endif