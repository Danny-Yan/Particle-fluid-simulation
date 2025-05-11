#pragma once

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "LTexture.h"

class Rendered
{
public:
	virtual void render(SDL_Renderer* gRenderer, LTexture& gDotTexture) = 0;
};

