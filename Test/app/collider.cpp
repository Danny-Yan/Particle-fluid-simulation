#include "incl/collider.h"


Collider::Collider(int x, int y, int radius, int fr)
    : fR(fr)
{
    //Initialize the offsets
    mCollider.x = x;
    mCollider.y = y;
    
    mCollider.r = radius * SCALE;
}

const Circle& Collider::getColliders() const
{
    return mCollider;
}

void Collider::setDensity( float density )
{
    this->density = density;
}

float Collider::getDensity() const
{
    return density;
}

void Collider::setfR( float fR )
{
    this->fR = fR;
}

float Collider::getfR() const
{
    return fR;
}

float Collider::getForceMultiplier() const
{
    return forceMultiplier;
}

void Collider::setForceMultiplier( float forceMultiplier )
{
    this->forceMultiplier = forceMultiplier;
}