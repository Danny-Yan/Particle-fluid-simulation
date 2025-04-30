#ifndef HELPER_MAIN_FUNCTIONS_H
#define HELPER_MAIN_FUNCTIONS_H

#define _USE_MATH_DEFINES
#include <math.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include "Dot.h"
#include "helper_structs.h"
#include "spatial_hashers.h"
#include <memory>
#include <functional>

// #include <boost/coroutine2/all.hpp>

// typedef boost::coroutines2::coroutine<Dot *> coro_t;

void updateSpatialLookup(std::vector<Entry> &spacialLookup, std::vector<int> &spacialKeys, std::vector<Dot> &circles);
void updateSpatialLookup(ParticleEntries &particleEntries);
void updateDensities(std::vector<Dot> &dots, std::vector<Entry> &particleHashEntries, std::vector<int> &spacialKeys);
void updateDensities(ParticleEntries& particleEntries);

void calculateDensity( float &particle_density, std::vector<Dot*> &circles, float x, float y );
float calcDensity(Dot& dotB, float x, float y);
float calcDensity(float magnitude);


void calculatePressureGradient( std::vector<float> &pressureGradient, Dot *dotB, Dot *dotA );
void calculatePressureGradient(std::vector<float>& pressureGradient, float magnitude, Dot* dotB, Dot* dotA);
float smoothingKernel( float distance, float radius);
float smoothingKernelDerivative( float distance, float radius);
std::vector<float> getRandomDirection();
float densityToPressure( float density );
void particleFilter( std::vector<Dot*> &filtered_dots, std::vector<Dot> &circles, std::vector<Entry> &particleHashEntries, std::vector<int> &spacialKeys, Dot &dot );

void forParticles(Dot& dotA, ParticleEntries& ParticleEntries, const std::function<void(Dot, float)>& func);
void mouseLeftPress( SDL_MouseButtonEvent &b, Mouse *mouse);
void mouseRightPress( SDL_MouseButtonEvent &b, Mouse *mouse);
void mouseUnPress( SDL_MouseButtonEvent &b, Mouse *mouse);

// Color processing
std::vector<Uint8> colourProcessor(int speed);
Uint8 colourLinearisation(int speed, int color_max_speed, int color_min, int color_max);

// // Particle filter iterator using boost
// void particleFilterIterator(coro_t::push_type& yield, std::vector<Dot> &circles, std::vector<Entry> &particleHashEntries, std::vector<int> &spacialKeys, Dot &dotA);
#endif