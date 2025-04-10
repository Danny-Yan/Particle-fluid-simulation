#ifndef MOUSE_H
#define MOUSE_H

#include <stdio.h>
#include "collider.h"

//Mouse structure
class Mouse : public Collider
{
    public:
        //Initializes the variables
        Mouse( int radius, int fR, float forceMultiplier = 0);

        int x; 
        int y;
        int r;
        int velX; 
        int velY;
        //Moves the mouse
        void move();
    
    protected:
        void shiftColliders() override;
};


#endif
