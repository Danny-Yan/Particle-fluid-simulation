#include "../../incl/Collider.h"

Collider::Collider(float x, float y)
{
	mPosX = x;
	mPosY = y;
}

float Collider::getmPosX() const
{
	return mPosX;
}

float Collider::getmPosY() const
{
	return mPosY;
}

void Collider::setmPosX(float val)
{
	mPosX = val;
}

void Collider::setmPosY(float val)
{
	mPosY = val;
}
