#ifndef HELPER_MAIN_FUNCTIONS_H
#define HELPER_MAIN_FUNCTIONS_H

#define _USE_MATH_DEFINES
#include <cmath>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <memory>
#include <cmath>
#include <array> 
#include <functional>

#include "incl/Dot.h"
#include "incl/helper_structs.h"
#include "incl/spatial_hashers.h"

// add namespace helper
[[deprecated("Use updateDensities(ParticleEntries& particleEntries) overload instead.")]]
void updateDensities(std::vector<Dot> &dots, std::vector<Entry> &particleHashEntries, std::vector<int> &spacialKeys);
void updateDensities(ParticleEntries& particleEntries);

[[deprecated("Use calcDensity() instead.")]]
void calculateDensity( float &particle_density, std::vector<Dot*> &circles, float x, float y );
float calcDensity(Dot& dotB, float x, float y);
float calcDensity(float magnitude);

void calculatePressureGradient( std::vector<float> &pressureGradient, Collider *dotB, Collider *dotA );
void calculatePressureGradient(std::vector<float>& pressureGradient, float magnitude, Dot* dotB, Dot* dotA);
float smoothingKernel( float distance, float radius);
float smoothingKernelDerivative( float distance, float radius);
std::vector<float> getRandomDirection();
float densityToPressure( float density );

[[deprecated("Use forParticles() or forParticlesAroundPoint() instead.")]]
void particleFilter( std::vector<Dot*> &filtered_dots, std::vector<Dot> &circles, std::vector<Entry> &particleHashEntries, std::vector<int> &spacialKeys, Dot &dot );

void forParticles(Dot& dotA, ParticleEntries& ParticleEntries, const std::function<void(Dot&)>& func);
void forParticlesAroundPoint(int x, int y, ParticleEntries& ParticleEntries, const std::function<void(Dot&)>& func);

// Mouse
void mouseLeftPress( SDL_MouseButtonEvent &b, Mouse *mouse);
void mouseRightPress( SDL_MouseButtonEvent &b, Mouse *mouse);
void mouseUnPress( SDL_MouseButtonEvent &b, Mouse *mouse);

#endif