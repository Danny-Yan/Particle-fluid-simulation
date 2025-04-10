#ifndef SPATIAL_HASHERS_H
#define SPATIAL_HASHERS_H
#include <vector>
#include "constants.h"
#include <tuple>
//Computing full 3x3 spacial area
std::vector<int> compute_full_spatial_area(int mPosX, int mPosY);

//Computing spacial coordinates for a given position
std::tuple<int, int> compute_spatial_coords(int mPosX, int mPosY);

//Computing hash value for a set of spacial coords
int compute_spatial_hash(int spatialX, int spatialY);

#endif