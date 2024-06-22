#ifndef MOUSE_H
#define MOUSE_H

#include <stdio.h>
#include "collider.h"

//Mouse structure
class Mouse : public Collider
{
    public:
        //Initializes the variables
        Mouse( int radius, int fR );

        // //Getters
        // int getVelX();
        // int getVelY();
        // int getPosX();
        // int getPosY();

        // //Setters
        // void setVelX( int velX );
        // void setVelY( int velY );

        int x = 0; 
        int y = 0;
        int r;
        int velX = 0; 
        int velY = 0;
};


#endif
