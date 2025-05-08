#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <cstdio>
#include <vector>
#include "Mouse.h"

namespace Helper
{
	void mouseLeftPress(SDL_MouseButtonEvent& b, Mouse* mouse);
	void mouseRightPress(SDL_MouseButtonEvent& b, Mouse* mouse);
	void mouseUnPress(SDL_MouseButtonEvent& b, Mouse* mouse);

    //Forward declaration of Dot class
    // class Dot;

    //Circle/Circle collision detector
    Collision checkCollision(Circle& a, Circle& b);

    //Circle/Box collision detector
    Collision checkCollision(Circle& a, SDL_Rect& b);

    // Collision checkCollision( Circle& a, Mouse mouse );

    // Collision checkCollision( Dot& a, Dot& b );

    Collision checkXCollision(Circle& a, SDL_Rect& b);

    Collision checkYCollision(Circle& a, SDL_Rect& b);

    //Calculates distance squared between two points
    float distanceSquared(float x1, float y1, float x2, float y2);

    //Convert any vector into a unit vector
    std::vector<float> unitVector(float x, float y);

    float sharedDensity(float density1, float density2);

    //Calculates the sign of a number
    int sgn(int val); // Turn into template
};

