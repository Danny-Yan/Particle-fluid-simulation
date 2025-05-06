#ifndef DOT_H
#define DOT_H

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include "incl/collision_check.h"
#include "incl/LTexture.h"
#include "incl/constants.h"
#include "incl/helper_structs.h"
#include "incl/spatial_hashers.h"
#include "incl/collider.h"
#include "incl/Mouse.h"

//The dot that will move around on the screen
class Dot : public Collider
{
    public:
        //Initializes the variables -- make x, y virtual variables
        Dot( float x, float y , float velX, float velY, int radius);

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
        float getmVelX() const;
        float getmVelY() const;
        float getVelX() const;
        float getVelY() const;
        float getmPosX() const;
        float getmPosY() const;
        float getPosX() const;
        float getPosY() const;
        float getsPosX() const;
        float getsPosY() const;
        int getID() const;

        //Setters
        void addmVelX( float velX );
        void addmVelY( float velY );

        //Shows the dot on the screen
        void render( SDL_Renderer *gRenderer, LTexture& gDotTexture);

        static int globalID;
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