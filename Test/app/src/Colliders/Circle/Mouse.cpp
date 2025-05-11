#include "../../../incl/Mouse.h"

Mouse::Mouse( int radius, int fR, float forceMultiplier ) : CircleCollider(0, 0, radius, fR)
{
    setForceMultiplier(forceMultiplier);
}

//Moves the mouse
void Mouse::move()
{
    this->shiftColliders();
	//printf("%f %f", mPosX, mPosY);
}

bool Mouse::hasBeenPressed() const {
	return isPressed;
}
void Mouse::setIsPressed(bool val) {
	isPressed = val;
}