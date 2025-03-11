#include "incl/Mouse.h"

Mouse::Mouse( int radius, int fR, float forceMultiplier ) : Collider(x, y, radius, fR)
{
    setForceMultiplier(forceMultiplier);
}

//Moves the mouse
void Mouse::move()
{
    this->shiftColliders();
}

void Mouse::shiftColliders()
{
    mCollider.x = x;
    mCollider.y = y;
}
