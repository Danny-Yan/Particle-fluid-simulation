#include "incl/collider.h"


Collider::Collider(int x, int y, int radius, int fr)
{
    this->fR = fr;

    //Initialize the offsets
    mCollider.x = x;
    mCollider.y = y;
    
    mCollider.r = radius * SCALE;
}

Circle& Collider::getColliders()
{
    return mCollider;
}

void Collider::setDensity( float density )
{
    this->density = density;
}

float Collider::getDensity()
{
    return density;
}

void Collider::setfR( float fR )
{
    this->fR = fR;
}

float Collider::getfR()
{
    return fR;
}

float Collider::getForceMultiplier()
{
    return forceMultiplier;
}

void Collider::setForceMultiplier( float forceMultiplier )
{
    this->forceMultiplier = forceMultiplier;
}