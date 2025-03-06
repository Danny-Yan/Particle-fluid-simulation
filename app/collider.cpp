#include "incl/collider.h"


Collider::Collider(int radius)
{
    mCollider.r = radius * SCALE;
    fR = FORCE_RADIUS;
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