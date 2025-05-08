#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <cstdio>
#include <vector>
#include "incl/Mouse.h"

namespace Helper
{
	void mouseLeftPress(SDL_MouseButtonEvent& b, Mouse* mouse);
	void mouseRightPress(SDL_MouseButtonEvent& b, Mouse* mouse);
	void mouseUnPress(SDL_MouseButtonEvent& b, Mouse* mouse);
};

