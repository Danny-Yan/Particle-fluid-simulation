#pragma once

#include <cstdio>
#include "CircleCollider.h"

//Mouse structure
class Mouse : public virtual CircleCollider
{
    public:
        //Initializes the variables
        Mouse( int radius, int fR, float forceMultiplier = 0);

        //Moves the mouse
        void move();
        bool hasBeenPressed() const;
        void setIsPressed(bool val);

    protected:
        bool isPressed = false;
        int velX;
        int velY;
};