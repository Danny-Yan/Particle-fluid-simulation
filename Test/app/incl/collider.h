#ifndef COLLIDER_H
#define COLLIDER_H

#include <cstdio>
#include <cmath>
#include <algorithm>
#include "incl/collision_check.h"
#include "incl/constants.h"

class Collider
{
    public:
        Collider(float x, float y, int radius, int fr = FORCE_RADIUS);
        const Circle& getColliders() const;
        void setDensity( float density );
        float getDensity() const;
        void setfR( float fR );
        float getfR() const;
        float getmPosX() const;
        float getmPosY() const;
        void setmPosX(float val);
        void setmPosY(float val);
        void setForceMultiplier( float forceMultiplier );
        float getForceMultiplier() const;

        float mPosX, mPosY;
    protected: 
        // Force radius
        float fR;

        // Density around the collider
        float density = 0.0f;

        //Force multipler
        float forceMultiplier = 1.0f;

        // Collision circle
        Circle mCollider;

        //Moves the collision circle
        void shiftColliders();
};

#endif