#include "incl/constants.h"

//Force constants
const float FORCE_RADIUS = 50.0f;           // ***
const float FORCE = 25;                    // ***

//Screen dimension constants
const int SCREEN_WIDTH = 1500;
const int SCREEN_HEIGHT = 800;
int GRID_WIDTH = (int)FORCE_RADIUS;
int GRID_HEIGHT = (int)FORCE_RADIUS;
float TIMEINTERVAL = 1.0f; // Min 1

//Dot constants
const float RADIUS = 10.0f;               // ***
const float SPACING = 5.0f;               // ***
const float accel = 0;               // Hella buggy
const float RESISTANCE = 1.0f;

const float SCALE = 0.5f;
const int PARTICLE_NUM = 2000;

// Get chrono to work
// convert particleFilter to an iterator class


