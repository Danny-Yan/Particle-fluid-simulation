#ifndef COLLISION_CHECK_H
#define COLLISION_CHECK_H

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdio.h>
#include <vector>

//A circle stucture
struct Circle
{
    int x, y;
    int r;
};

//A collision vector
struct Collision
{
    std::vector<float> v;
    std::vector<float> sa;
    std::vector<float> sb;
    bool didCollide = false;
};

//Circle/Circle collision detector
Collision checkCollision( Circle& a, Circle& b );

//Circle/Box collision detector
Collision checkCollision( Circle& a, SDL_Rect& b );

//Calculates distance squared between two points
double distanceSquared( int x1, int y1, int x2, int y2 );

//Calculates the sign of a number
int sgn( int val ); // Turn into template

#endif