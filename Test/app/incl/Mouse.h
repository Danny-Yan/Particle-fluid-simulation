#ifndef MOUSE_H
#define MOUSE_H

#include <cstdio>
#include "incl/collider.h"

//Mouse structure
class Mouse : public virtual Collider
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


#endif
