#include "incl/constants.h"

//Force constants
const float FORCE_RADIUS = 10.0f;           // ***
const float FORCE = 1;                    // ***

//Screen dimension constants
const int SCREEN_WIDTH = 1500;
const int SCREEN_HEIGHT = 800;
int GRID_WIDTH = (int)FORCE_RADIUS;
int GRID_HEIGHT = (int)FORCE_RADIUS;
float TIMEINTERVAL = 1.0f; // Min 1

//Dot constants
const float RADIUS = 10.0f;               // ***
const float SPACING = 3.0f;               // ***
const float accel = 0;               // Hella buggy
const float RESISTANCE = 0.9f;

const float SCALE = 0.3f;
const int PARTICLE_NUM = 500;


//Check:
// Inside of check_vector_collision
// checkXCollision
// checkYCollision
// Forces are not working (dot dont touch screen width nor screen height)

// Get chrono to work
// convert particleFilter to an iterator class


