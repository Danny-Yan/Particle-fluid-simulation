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

    protected: 
        float fR;
        float density;

        // Collision circle
        Circle mCollider;
};

#endif