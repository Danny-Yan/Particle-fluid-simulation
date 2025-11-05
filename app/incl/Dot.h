#pragma once

#include <cstdio>
#include <cmath>
#include <algorithm>
#include "Helper.h"
#include "constants.h"
#include "helper_structs.h"
#include "CircleCollider.h"
#include "Mouse.h"
#include "Rendered.h"

//The dot that will move around on the screen
class Dot : public CircleCollider, public virtual Rendered
{
    public:
        //Initializes the variables -- make x, y virtual variables
        Dot( float x, float y , float velX, float velY, int radius);

        //Moves the dot
        void movePrediction( float deltaTime, float constantDelta );
        void move( float deltaTime );

        //Check collision with another dot  ----------- REFACTOR OUT EVENTUALLY
        void check_vector_force( Dot &dot );
        void check_wall_collision();
        void check_wall_no_shift();
        void applyDotCollison(Dot &dot);
        void applyDotCollisons(std::vector<Dot>& dots);
        void applyDotCollisons(std::vector<Dot*>& dots);
        void check_mouse_force( Mouse *mouse );
        Collision checkCircleForce( CircleCollider& b );
        // -----------------------------------------------------------------

        //Getters
        float getmVelX() const;
        float getmVelY() const;
        float getVelX() const;
        float getVelY() const;
        float getPosX() const;
        float getPosY() const;
        float getsPosX() const;
        float getsPosY() const;
        int getID() const;

        //Setters
        void addmVelX( float velX );
        void addmVelY( float velY );

        //Shows the dot on the screen
        void render( SDL_Renderer *gRenderer, LTexture& gDotTexture) override;

        static int globalID;
    private:
        // Dot ID:
        int id;

        //The X and Y coords
        float PosX, PosY;

		//Predicted X and Y coords
        float sPosX, sPosY;

        //The velocity of the dot
        float mVelX, mVelY;

        // Velocity of dot from prev frame
        float Velx, Vely;

        //Dot's collision vector with another dot
        Collision cLVector;
        Collision checkDotCollision( Dot& b );
};