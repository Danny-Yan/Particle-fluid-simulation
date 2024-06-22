#ifndef HELPER_MAIN_FUNCTIONS_H
#define HELPER_MAIN_FUNCTIONS_H

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include "Dot.h"
#include "helper_structs.h"
#include "spatial_hashers.h"

void updateSpatialLookup(std::vector<Entry> &spacialLookup, std::vector<int> &spacialKeys, std::vector<Dot> &circles);
void calculateDensity( float &particle_density, std::vector<Dot*> &circles, Dot &dot );
void particleFilter( std::vector<Dot*> &filtered_dots, std::vector<Dot> &circles, std::vector<Entry> &particleHashEntries, std::vector<int> &spacialKeys, Dot &dot );
#endif