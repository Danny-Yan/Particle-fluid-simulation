#include "../../../incl/CircleCollider.h"


CircleCollider::CircleCollider(float x, float y, int radius, int fr)
	: fR(fr), Collider(x, y)
{
    //Initialize the offsets
    this->mCollider.x = (int)x;
    this->mCollider.y = (int)y;
    this->mCollider.r = radius * SCALE;
}

const Circle& CircleCollider::getColliders() const
{
    return this->mCollider;
}

void CircleCollider::setDensity( float density )
{
    this->density = density;
}

float CircleCollider::getDensity() const
{
    return density;
}

void CircleCollider::setfR( float fR )
{
    this->fR = fR;
}

float CircleCollider::getfR() const
{
    return fR;
}

float CircleCollider::getForceMultiplier() const
{
    return forceMultiplier;
}

void CircleCollider::setForceMultiplier( float forceMultiplier )
{
    this->forceMultiplier = forceMultiplier;
}

//SHIFT COLLIDERS
void CircleCollider::shiftColliders()
{
    //Align collider to center of dot
    this->mCollider.x = mPosX;
    this->mCollider.y = mPosY;
}