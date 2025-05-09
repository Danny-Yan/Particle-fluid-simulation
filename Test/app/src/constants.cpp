#include "../incl/constants.h"

//Force constants
const float FORCE_RADIUS = 40.0f;           // ***
const float FORCE = 40;                    // ***

const float FORCE_RADIUS_SQUARED = FORCE_RADIUS * FORCE_RADIUS;

//Screen dimension constants
const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 920;
const int PARTICLE_START_X = 700;
const int PARTICLE_START_Y = 600;
int GRID_WIDTH = (int)FORCE_RADIUS;
int GRID_HEIGHT = (int)FORCE_RADIUS;
int SPATIAL_WIDTH = SCREEN_WIDTH / GRID_WIDTH;
int SPATIAL_HEIGHT = SCREEN_HEIGHT / GRID_HEIGHT;

// Simulation Constants
float TIMEINTERVAL = 1.0f; // Min 1 ms
float PREDICTIVE_STEPS = 0.128f;

//Dot constants
const float RADIUS = 5.0f;               // ***
const float SPACING = 5.0f;               // ***
const float SCALE = 0.4f;
const int PARTICLE_NUM = 7000; // does not work for < 200

const float RADIUS_SQUARED = RADIUS * RADIUS;

// Motion constants
const float accel = 0;               // Hella buggy
const float RESISTANCE = 0.1f;      // *** UNSTABLE AT r = 1

// MOUSE
const int MOUSE_FORCE_RADIUS = (int)FORCE_RADIUS * 4;
const float MOUSE_RADIUS_SQUARED = MOUSE_FORCE_RADIUS * MOUSE_FORCE_RADIUS;
const float MOUSE_FORCE = 3.0f;

// CONTACT
// CONVERT TO CONTACT FORCE
// CHANGE ALL sPos to Pos inside helper main
// Switch to contact force code
// force = 1
// force-radius = 50

// FORCE
// float BARRIER_WIDTH = FORCE_RADIUS;
// const float BARRIER_HEIGHT = 0.5f;  // Energy added/lost to the system

//Density Constants -- DONT TOUCH
const float DENSITY_LOWER = 0.0001f;
const float DENSITY_DESIRED = 0.00095f; // 0.0005f;
const float DENSITY_UPPER = 0.001f;


// Colour constants
const float COLOR_MAX_SPEED = 20;
const float COLOR_MIN[3] = {0, 249, 241};
const float COLOR_MAX[3] = {238, 86, 86};

// Hsl Constants
const float COLOR_H_MIN = 215;
const float COLOR_H_MAX = 1;