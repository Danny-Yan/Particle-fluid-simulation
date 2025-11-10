#pragma once

#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <functional>

#include "Dot.h"
#include "constants.h"
#include "LTexture.h"
#include "LTimer.h"
#include "Mouse.h"
#include "Helper.h"
#include "helper_structs.h"
#include "Rectangle.h"
#include "ParticleManager.h"


class engine
{
public:
	engine();

	// Main running function
	void run();

private:
	//The window we'll be rendering to
	SDL_Window* gwindow = NULL;

	//The window renderer
	SDL_Renderer* gRenderer = NULL;
	
	//Scene textures
	LTexture gDotTexture;

	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	// ParticleManager
	ParticleManager particleManager = ParticleManager(PARTICLE_NUM);

	//Set the wall
	Rectangle wall = Rectangle(400, 500, 0, 0);

	// Mouse
	Mouse mouse = Mouse(FORCE_RADIUS, MOUSE_FORCE_RADIUS);

	//Timer
	Uint64 Ticks = 0;
	Uint64 deltaTime = 0;
	Uint64 interval = 0;
	LTimer timer;

	// Colours
	Uint8 r = 0;
	Uint8 g = 0;
	Uint8 b = 0;

	std::vector<float> pressureGradient = { 0, 0 };

	//Starts up SDL and creates window
	bool initSDL();

	//Loads media
	bool loadMedia();

	bool start();
	bool isRunning();

	//Frees media and shuts down SDL
	void close();

	// Poll I/O events
	void pollEvent();

	// Clear screen
	void clearScreen();

	// Run simulation
	void whileRunning( const std::function<void()>& func);

	// Simulation frames
	// Fluid w/ particle manager
	void mGeneralSimSetUp();
	void mRunFluidSimFrame();

	void runFluidParticlesFrame();
	void runFluidMouseForceFrame();
	void runFluidMouseDensityFrame();
	void runSimRenderFrame(int timeInterval);

	//Collision
	void runBallCollisionFrame();
};