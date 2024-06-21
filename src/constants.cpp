#include "constants.h"

//Screen dimension constants
extern const int SCREEN_WIDTH = 1500;
extern const int SCREEN_HEIGHT = 800;
extern const int GRID_WIDTH = 10;
extern const int GRID_HEIGHT = 10;

//Dot constants
extern const float accel = 0;               // Hella buggy
extern const float vfriction = 0;           // min 0
extern const float hFriction = 0;           // Doesnt work :(

extern const float SCALE = 0.2f;
extern const float HITBOX_SCALE = 0.2f;
extern const int PARTICLE_NUM = 1000;

extern const float FORCE_RADIUS = 10.0f;
extern const float FORCE = 1;

extern const float BARRIER_WIDTH = 10.0f;
extern const float BARRIER_HEIGHT = 1.0f;  // Energy added/lost to the system

extern const float DENSITY_DESIRED = 1.0f;

//Check:
// Inside of check_vector_collision
// checkXCollision
// checkYCollision
// Forces are not working (dot dont touch screen width nor screen height)
