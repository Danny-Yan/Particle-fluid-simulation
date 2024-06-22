#include "collider.h"


Collider::Collider(int radius)
{
    mCollider.r = radius * HITBOX_SCALE;
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

void Collider::setfR( float fR )
{
    this->fR = fR;
}

float Collider::getfR()
{
    return fR;
}