#ifndef ENGINE_H
#define ENGINE_H

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <vector>
#include <functional>

#include "incl/Dot.h"
#include "incl/constants.h"
#include "incl/LTexture.h"
#include "incl/time.h"
#include "incl/Mouse.h"
#include "incl/collision_check.h"
#include "incl/helper_structs.h"
#include "incl/helper_main_functions.h"

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

	// Initialising vectors

	ParticleEntries particleEntries = ParticleEntries();
	std::vector<Entry> particleHashEntries = std::vector<Entry>(PARTICLE_NUM);
	std::vector<int> spacialKeys = std::vector<int>(PARTICLE_NUM);
	std::vector<Dot> dots;
	std::vector<Dot*> filtered_dots;

	//Set the wall
	SDL_Rect wall;

	//Timer
	Uint64 Ticks = 0;
	Uint64 deltaTime = 0;
	Uint64 interval = 0;
	LTimer timer;

	Mouse mouse = Mouse(FORCE_RADIUS, MOUSE_FORCE_RADIUS);

	// Colours
	Uint8 r = 0;
	Uint8 g = 0;
	Uint8 b = 0;

	std::vector<float> pressureGradient = { 0, 0 };

	//Starts up SDL and creates window
	bool initSDL();

	//Loads media
	bool loadMedia();

	//Frees media and shuts down SDL
	void close();

	// Poll I/O events
	void pollEvent();

	// Clear screen
	void clearScreen();

	// Set up particle simulation
	void generalSimulationSetUp();
	void generalSimSetUp();

	void fluidSimulationSetUp();
	void collisionSimulationSetUp();

	// Run simulation
	void whileRunning( const std::function<void()>& func);

	// Simulation frames
	void runFluidSimulationFrame();
	void runFluidSimFrame();
	void runBallCollisionFrame();
};

#endif

