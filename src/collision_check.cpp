#include "collision_check.h"

//Check collision between two circles
Collision checkCollision( Circle& a, Circle& b )
{
    Collision collision;
    //Calculate total radius squared
    int totalRadiusSquared = a.r + b.r;
    totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;

    //If the distance between the centers of the circles is less than the sum of their radii
    if( distanceSquared( a.x, a.y, b.x, b.y ) < ( totalRadiusSquared ) )
    {
        if (a.x - b.x == 0){
            return collision;
        }

        //The circles have collided
        collision.v.push_back(a.x - b.x);
        collision.v.push_back(a.y - b.y);
        collision.didCollide = true;
        return collision;
    }

    //If not
    return collision;
}

//Check collision between circle and rectangle
Collision checkCollision( Circle& a, SDL_Rect& b )
{
    //Closest point on collision box
    int cX, cY;

    Collision collision;

    //Find closest x offset
    if( a.x < b.x )
    {
        cX = b.x;
    }
    else if( a.x > b.x + b.w )
    {
        cX = b.x + b.w;
    }
    else
    {
        cX = a.x;
    }
    
    //Find closest y offset
    if( a.y < b.y )
    {
        cY = b.y;
    }
    else if( a.y > b.y + b.h )
    {
        cY = b.y + b.h;
    }
    else
    {
        cY = a.y;
    }

    //If the closest point is inside the circle
    if( distanceSquared( a.x, a.y, cX, cY ) < a.r * a.r )
    {
        //This box and the circle have collided
        collision.didCollide = true;
        return collision;
    }

    //If the shapes have not collided
    return collision;
}

//Calculate distance squared between two points
double distanceSquared( int x1, int y1, int x2, int y2 )
{
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    return deltaX*deltaX + deltaY*deltaY;
}

int sgn(int val) {
    return (0 < val) - (val < 0);
}