#include "constants.h"

//Force constants
const float FORCE_RADIUS = 100.0f;           // ***
const float FORCE = 100;                    // ***

//Screen dimension constants
const int SCREEN_WIDTH = 1500;
const int SCREEN_HEIGHT = 800;
int GRID_WIDTH = (int)FORCE_RADIUS;
int GRID_HEIGHT = (int)FORCE_RADIUS;
float TIMEINTERVAL = 1.0f; // Min 1


//Dot constants
const float accel = 0;               // Hella buggy
const float vfriction = 0;           // min 0
const float hFriction = 0;           // Doesnt work :(

const float SCALE = 0.3f;
const float HITBOX_SCALE = 1.0f;
const int PARTICLE_NUM = 2000;

//Density Constants
float BARRIER_WIDTH = FORCE_RADIUS;
const float BARRIER_HEIGHT = 1.0f;  // Energy added/lost to the system

const float DENSITY_DESIRED = 0.00005f;      // Reallllllllly weird 0.0001f
//Starting density: 0.007 - 0.009

//Check:
// Inside of check_vector_collision
// checkXCollision
// checkYCollision
// Forces are not working (dot dont touch screen width nor screen height)




// Get chrono to work
// convert particleFilter to an iterator class


