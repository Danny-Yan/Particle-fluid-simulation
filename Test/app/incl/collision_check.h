#ifndef COLLISION_CHECK_H
#define COLLISION_CHECK_H

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <stdio.h>
#include <vector>
#include <map>

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
    bool didCollide = false;
};

//Forward declaration of Dot class
// class Dot;

//Circle/Circle collision detector
Collision checkCollision( Circle& a, Circle& b );

//Circle/Box collision detector
Collision checkCollision( Circle& a, SDL_Rect& b );

// Collision checkCollision( Circle& a, Mouse mouse );

// Collision checkCollision( Dot& a, Dot& b );

Collision checkXCollision( Circle& a, SDL_Rect& b );

Collision checkYCollision( Circle& a, SDL_Rect& b );

//Calculates distance squared between two points
float distanceSquared( int x1, int y1, int x2, int y2 );

//Convert any vector into a unit vector
std::vector<float> unitVector(float x, float y);

float sharedDensity( float density1, float density2 );


//Calculates the sign of a number
int sgn( int val ); // Turn into template

// std::map<int, int> mapping_spacial;
// void initMap();

#endif