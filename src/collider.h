#ifndef COLLIDER_H
#define COLLIDER_H

#include <stdio.h>
#include <cmath>
#include <algorithm>
#include "collision_check.h"
#include "constants.h"


class Collider
{
    public:
        Collider(int radius);
        Circle& getColliders();
        void setDensity( float density );
        void setfR( float fR );
        float getfR();
        void setForceMultiplier( float forceMultiplier );
        float getForceMultiplier();

    protected: 
        // Force radius
        float fR;

        // Density around the collider
        float density;

        //Force multipler
        float forceMultiplier = 1.0f;

        // Collision circle
        Circle mCollider;
        
        //Moves the collision circle relative to the dot's offset
        virtual void shiftColliders() = 0;
};

#endif