#ifndef DOT_H
#define DOT_H

#define SDL_MAIN_HANDLED
#include <SDL.h>
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
        //Initializes the variables -- make x, y virtual variables
        Dot( int x, int y , float velX, float velY, int radius);

        //Moves the dot
        void movePrediction( float deltaTime, float constantDelta );
        void move( float deltaTime );

        // Previous design
        void moveVector( float deltaTime );

        //Check collision with another dot
        void check_vector_collision( float deltaTime, SDL_Rect& square, std::vector<Dot>& circles, std::vector<Entry>& particleHashEntries, std::vector<int> &spacialKeys, int index );
        void check_vector_force( Dot &dot );
        void check_wall_collision();
        void check_wall_no_shift();
        void applyDotCollison(Dot &dot);
        void applyDotCollisons(std::vector<Dot>& dots);
        void applyDotCollisons(std::vector<Dot*>& dots);
        void check_mouse_force( Mouse *mouse );
        Collision checkCircleForce( Collider& b );

        //Getters
        float getmVelX();
        float getmVelY();
        float getVelX();
        float getVelY();
        float getmPosX();
        float getmPosY();
        float getPosX();
        float getPosY();
        float getsPosX();
        float getsPosY();

        int getID();

        //Setters
        void addmVelX( float velX );
        void addmVelY( float velY );

        //Shows the dot on the screen
        void render( SDL_Renderer *gRenderer, LTexture& gDotTexture);

        static int ID;

    private:
        // Dot ID:
        int id;

        //The X and Y offsets of the dot
        float mPosX, mPosY;
        float PosX, PosY;
        float sPosX, sPosY;

        //The velocity of the dot
        float mVelX, mVelY;
        float Velx, Vely;

        //Dot's collision vector with another dot
        Collision cLVector;

        void shiftColliders() override;
        void shiftCollidersPos();
        Collision checkDotCollision( Dot& b );
};


#endif