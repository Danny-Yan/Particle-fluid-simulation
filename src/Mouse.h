#ifndef MOUSE_H
#define MOUSE_H

#include <stdio.h>
#include "collider.h"

//Mouse structure
class Mouse : public Collider
{
    public:
        //Initializes the variables
        Mouse( int radius, int fR, float forceMultiplier);

        int x = 0; 
        int y = 0;
        int r;
        int velX = 0; 
        int velY = 0;
        //Moves the mouse
        void move();
    
    protected:
        void shiftColliders() override;
};


#endif
