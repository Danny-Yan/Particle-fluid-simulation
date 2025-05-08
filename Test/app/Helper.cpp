#include "incl/Helper.h"

// MOUSE HANDLES
void Helper::mouseLeftPress(SDL_MouseButtonEvent& b, Mouse* mP) {
    Mouse& mouse = *mP;
    mouse.setIsPressed(true);
    mouse.setForceMultiplier(MOUSE_FORCE);
}

// Right click
void Helper::mouseRightPress(SDL_MouseButtonEvent& b, Mouse* mP) {
    Mouse& mouse = *mP;
    mouse.setIsPressed(true);
    mouse.setForceMultiplier(-1 * MOUSE_FORCE);
}

// Left click
void Helper::mouseUnPress(SDL_MouseButtonEvent& b, Mouse* mP) {
    Mouse& mouse = *mP;
    mouse.setIsPressed(false);
    mouse.setForceMultiplier(0);
}