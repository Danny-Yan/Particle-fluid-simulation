#include "../../incl/Helper.h"

// MOUSE HANDLES
void Helper::mouseLeftPress(SDL_MouseButtonEvent& b, Mouse* mP) {
    Mouse& mouse = *mP;
    mouse.setIsPressed(true);
    mouse.setForceMultiplier(MOUSE_FORCE);
}

// Right click
void Helper::mouseRightPress(SDL_MouseButtonEvent& b, Mouse* mP) {
    Mouse& mouse = *mP;
    mouse.setIsPressed(true);
    mouse.setForceMultiplier(-1 * MOUSE_FORCE);
}

// Left click
void Helper::mouseUnPress(SDL_MouseButtonEvent& b, Mouse* mP) {
    Mouse& mouse = *mP;
    mouse.setIsPressed(false);
    mouse.setForceMultiplier(0);
}

//Check collision between two circles
Collision Helper::checkCollision( Circle& a, Circle& b )
{
    Collision collision;
    //Calculate total radius squared
    int totalRadiusSquared = a.r + b.r;
    totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;

    //If the distance between the centers of the circles is less than the sum of their radii
    if( distanceSquared( a.x, a.y, b.x, b.y ) < ( totalRadiusSquared ) )
    {
        collision.didCollide = true;
        return collision;
    }

    //If not
    return collision;
}

//Check collision between circle and rectangle
Collision Helper::checkCollision( Circle& a, SDL_Rect& b )
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

// Collision checkCollision( Circle& a, Mouse b )
// {
//     Collision collision;
//     //Calculate total radius squared
//     int radius = a.r + b.r;
//     int totalRadiusSquared = radius * radius;

//     //If the distance between the centers of the circles is less than the sum of their radii
//     if( distanceSquared( a.x, a.y, b.x, b.y ) < ( totalRadiusSquared ) )
//     {
//         collision.didCollide = true;
//         return collision;
//     }

//     //If not
//     return collision;

// }

Collision Helper::checkXCollision( Circle& a, SDL_Rect& b )
{
    //Closest point on collision box
    int cX;

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

    //If the closest point is inside the circle
    if( abs(cX - a.x) < a.r)
    {
        //This box and the circle have collided
        collision.didCollide = true;
        return collision;
    }

    //If the shapes have not collided
    return collision;
}

Collision Helper::checkYCollision( Circle& a, SDL_Rect& b )
{
    //Closest point on collision box
    int cY;

    Collision collision;

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
    if( abs(cY - a.y) < a.r)
    {
        //This box and the circle have collided
        collision.didCollide = true;
        return collision;
    }

    //If the shapes have not collided
    return collision;
}

//Calculate distance squared between two points
float Helper::distanceSquared( float x1, float y1, float x2, float y2 )
{
    float deltaX = x2 - x1;
    float deltaY = y2 - y1;
    return deltaX*deltaX + deltaY*deltaY;
}

std::vector<float> Helper::unitVector(float x, float y)
{
    float magnitude = sqrt(x * x + y * y);
    float x_unit = x / magnitude;
    float y_unit = y / magnitude;
    std::vector<float> v = {x_unit, y_unit};
    return v;
}

float Helper::sharedDensity( float density1, float density2 )
{
    return (density1 + density2) / 2;

}

int Helper::sgn(int val) {
    return (0 < val) - (val < 0);
}