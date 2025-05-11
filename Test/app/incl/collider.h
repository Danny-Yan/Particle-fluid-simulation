#pragma once
#include "Shape.h"
class Collider
{
public:
    Collider(float x, float y);
    float getmPosX() const;
    float getmPosY() const;
    void setmPosX(float val);
    void setmPosY(float val);
    //virtual const Shape& getColliders() const = 0;

protected:
	// Position of the collider
	float mPosX, mPosY;
    //Shape mCollider;
};

