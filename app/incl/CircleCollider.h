#pragma once

#include <cstdio>
#include <cmath>
#include <algorithm>
#include "helper_structs.h"
#include "constants.h"
#include "Collider.h"

class CircleCollider : public Collider
{
    public:
        CircleCollider(float x, float y, int radius, int fr = FORCE_RADIUS);
        //const Shape& getColliders() const override;
        const Circle& getColliders() const;
        void setDensity( float density );
        float getDensity() const;
        void setfR( float fR );
        float getfR() const;
        void setForceMultiplier( float forceMultiplier );
        float getForceMultiplier() const;
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