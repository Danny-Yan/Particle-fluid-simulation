#include "../../incl/collider.h"


Collider::Collider(float x, float y, int radius, int fr)
	: fR(fr), mPosX(x), mPosY(y)
{
    //Initialize the offsets
    this->mCollider.x = (int)x;
    this->mCollider.y = (int)y;
    this->mCollider.r = radius * SCALE;
}

const Circle& Collider::getColliders() const
{
    return this->mCollider;
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

float Collider::getmPosX() const
{
    return mPosX;
}

float Collider::getmPosY() const
{
    return mPosY;
}

void Collider::setmPosX(float val) {
    this->mPosX = val;
}

void Collider::setmPosY(float val) {
    this->mPosY = val;
}

//SHIFT COLLIDERS
void Collider::shiftColliders()
{
    //Align collider to center of dot
    this->mCollider.x = mPosX;
    this->mCollider.y = mPosY;
}
