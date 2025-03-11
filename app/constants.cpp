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
const int PARTICLE_NUM = 2000; // does not work for < 200

// MOUSE
const int MOUSE_FORCE_RADIUS = (int)FORCE_RADIUS;
const float MOUSE_FORCE = -0.1f;


// CONTACT
// CONVERT TO CONTACT FORCE
// CHANGE ALL sPos to Pos inside helper main
// Switch to contact force code
// force = 1
// force-radius = 50

// FORCE
// float BARRIER_WIDTH = FORCE_RADIUS;
// const float BARRIER_HEIGHT = 0.5f;  // Energy added/lost to the system

//Density Constants
const float DENSITY_LOWER = 0.0000000001f;
const float DENSITY_DESIRED = 0.00000005f;
const float DENSITY_UPPER = 0.0001f;


// Colour constants
const float COLOR_MAX_SPEED = 1000; // speed * 100
const float COLOR_MIN[3] = {0, 249, 241};
const float COLOR_MAX[3] = {238, 86, 86};