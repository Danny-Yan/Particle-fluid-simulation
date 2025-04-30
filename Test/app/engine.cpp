#include "incl/engine.h"


engine::engine()
{
    wall.x = 500;
    wall.y = 400;
    wall.w = 0;
    wall.h = 0;
}

bool engine::initSDL()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
        return success;
    }

    //Create window
    gwindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gwindow == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        success = false;
        return success;
    }

    //Create renderer for window
    gRenderer = SDL_CreateRenderer(gwindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (gRenderer == NULL)
    {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        success = false;
        return success;
    }
        //Initialize renderer color
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        success = false;
        return success;
    }

    return success;
}

bool engine::loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load Foo' texture
    if (!gDotTexture.loadFromFile(gRenderer, "images/transparent_dot.png"))
    {
        printf("Failed to load dot' texture image!\n");
        success = false;
    }

    return success;
}

void engine::close()
{
    //Free loaded images
    gDotTexture.free();

    //Destroy window    
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gwindow);
    gwindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

void engine::run()
{
    //Start up SDL and create window
    if (!initSDL())
    {
        printf("Failed to initialize!\n");
        return;
    }

    //Load media
    if (!loadMedia())
    {
        printf("Failed to load media!\n");
        return;
    }

    timer.start();
    // Setup simulation
    generalSimSetUp();

    // Run simulation
	whileRunning([&](){ runFluidSimFrame();});

	// Close simulation when done
    close();
}

void engine::generalSimulationSetUp()
{
    float spacing_scale = RADIUS * SCALE + SPACING;
    int particlesPerRow = (int)sqrt(PARTICLE_NUM);
    int particlesPerCol = (PARTICLE_NUM - 1) / particlesPerRow + 1;
    float x_cord;
    float y_cord;
    float particle_density;

    for (int i = 0; i < PARTICLE_NUM; i++)
    {
        // radius = std::rand() % 10 + 5;
        float x = (i % particlesPerRow - particlesPerRow / 2.0f + 0.5f) * (spacing_scale);
        float y = (i / particlesPerRow - particlesPerCol / 2.0f + 0.5f) * (spacing_scale);

        x_cord = x + PARTICLE_START_X;
        y_cord = y + PARTICLE_START_Y;
        Dot dot(x_cord, y_cord, 0, 0, RADIUS);
        dots.push_back(dot);
    }

    for (int i = 0; i < PARTICLE_NUM; i++)
    {
        Dot& dot = dots[i];
        // Filters dots only within the spatial vicinity
        particleFilter(filtered_dots, dots, particleHashEntries, spacialKeys, dot); // filtered_dots

        // Calc density of a position (particle position)
        calculateDensity(particle_density, filtered_dots, dot.getPosX(), dot.getPosY());
        dot.setDensity(particle_density);
    }
}

void engine::generalSimSetUp() {
    float spacing_scale = RADIUS * SCALE + SPACING;
    int particlesPerRow = (int)sqrt(PARTICLE_NUM);
    int particlesPerCol = (PARTICLE_NUM - 1) / particlesPerRow + 1;
    float x_cord;
    float y_cord;
    float particle_density;

    for (int i = 0; i < PARTICLE_NUM; i++)
    {
        // radius = std::rand() % 10 + 5;
        float x = (i % particlesPerRow - particlesPerRow / 2.0f + 0.5f) * (spacing_scale);
        float y = (i / particlesPerRow - particlesPerCol / 2.0f + 0.5f) * (spacing_scale);

        x_cord = x + PARTICLE_START_X;
        y_cord = y + PARTICLE_START_Y;
        Dot dot(x_cord, y_cord, 0, 0, RADIUS);
        particleEntries.circles.push_back(dot);
    }

    updateDensities(particleEntries);
}
void engine::fluidSimulationSetUp(){}
void engine::collisionSimulationSetUp(){}

void engine::whileRunning(const std::function<void()>& func)
{
    while (!quit) {
        pollEvent();
        clearScreen();

        //Render wall
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderDrawRect(gRenderer, &wall);

        float deltaTime = (timer.getTicks() - interval) * 100000 / (float)SDL_GetPerformanceFrequency(); // Get the time passed in ms since the start of the frame.        
        // printf("%f\n", deltaTime);
        if (deltaTime >= TIMEINTERVAL && timer.isPaused() == 0)
        {
           func();
        }
    }
}

void engine::pollEvent()
{
    //Handle events on queue
    while (SDL_PollEvent(&e) != 0)
    {
        switch (e.type) {
            // QUIT
        case SDL_QUIT:
            quit = true;
            break;

            // MOUSE MOVE
        case SDL_MOUSEMOTION:
            SDL_GetMouseState(&mouse.x, &mouse.y);
            mouse.move();
            break;

            // MOUSE CLICK
        case SDL_MOUSEBUTTONDOWN:
            switch (e.button.button) {
            case SDL_BUTTON_LEFT:
                mouseLeftPress(e.button, &mouse);
                break;
            case SDL_BUTTON_RIGHT:
                mouseRightPress(e.button, &mouse);
                break;

            }
            break;

        case SDL_MOUSEBUTTONUP:
            mouseUnPress(e.button, &mouse);
            break;

            // KEYBOARD
        case SDL_KEYDOWN:
            switch (e.key.keysym.sym) {

                // Start/stop
            case SDLK_s:
                if (timer.isStarted())
                {
                    timer.stop();
                }
                else
                {
                    timer.start();
                }
                break;

                // Pause/Unpause
            case SDLK_p:
                if (timer.isPaused())
                {
                    timer.resetTicked();
                    timer.unpause();
                }
                else
                {
                    timer.pause();
                }
                break;

                // Ticked
            case SDLK_t:
                if (timer.isPaused())
                {
                    timer.unpause();
                    timer.setTicked();
                }
                break;
            }
        }
    }

}
void engine::clearScreen() {
    //Clear screen
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(gRenderer);
}
void engine::runFluidSimulationFrame()
{
    //DENSITY METHOD w/ PREDICTIVE STEPS -------------------------------------------------------------------------------------------------------

    //Move all dots
    for (int i = 0; i < PARTICLE_NUM; i++)
    {
        Dot& dot = dots[i];
        dot.movePrediction(TIMEINTERVAL, 0.5f);
    }

    // Update spacial lookup after moving
    updateSpatialLookup(particleHashEntries, spacialKeys, dots);

    //Update densities after moving
    updateDensities(dots, particleHashEntries, spacialKeys);

    for (int i = 0; i < PARTICLE_NUM; i++)
    {
        Dot& dot = dots[i];
        dot.check_wall_no_shift();

        std::vector<float> pressureGradient = { 0, 0 };

        // Finds all dots within a vicinity
        //Computing 3x3 spacial hash 
        std::vector<int> spacial_hashes_full = compute_full_spatial_area((int)dot.getsPosX(), (int)dot.getsPosY());
        //Iterating through spacial hashes
        for (int hash : spacial_hashes_full)
        {
            int key = spacialKeys[hash];

            for (int i = key; i < dots.size(); i++)
            {
                if (particleHashEntries[i].hash != hash) {
                    break;
                }

                Dot& dotB = dots[particleHashEntries[i].index];
                float distance_squared = distanceSquared(dot.getmPosX(), dot.getmPosY(), dotB.getmPosX(), dotB.getmPosY());

                if (&dotB == &dot || distance_squared >= FORCE_RADIUS_SQUARED) { continue; }
                // dot.applyDotCollison( dotB );

                // Calc mouse force
                //dot.check_mouse_force( &mouse );

                // Calc pressure gradient (Loop and keep track of pressure gradient)
                calculatePressureGradient(pressureGradient, &dotB, &dot);
            }
        }

        if (abs(dot.getDensity()) > DENSITY_UPPER)
        {
            dot.addmVelX(pressureGradient[0] / dot.getDensity());
            dot.addmVelY(pressureGradient[1] / dot.getDensity());
        }

        //// COLORING
        //int speed = (abs(dot.getVelX()) + abs(dot.getVelY())) * 100;
        //std::vector<Uint8> colors = colourProcessor(speed);
        //gDotTexture.setColor(colors[0], colors[1], colors[2]);

        //// RENDERING
        //dot.render(gRenderer, gDotTexture);
    }

    ////Move all dots
    for (Dot& dot : dots)
    {
        dot.move(TIMEINTERVAL);
        // COLORING
        int speed = (abs(dot.getVelX()) + abs(dot.getVelY())) * 100;
        std::vector<Uint8> colors = colourProcessor(speed);
        gDotTexture.setColor(colors[0], colors[1], colors[2]);

        // RENDERING
        dot.render(gRenderer, gDotTexture);
    }

    // ========================================================================================================================

    //Update screen
    SDL_RenderPresent(gRenderer);
    interval = timer.getTicks();
}


void engine::runFluidSimFrame()
{
    //DENSITY METHOD w/ PREDICTIVE STEPS -------------------------------------------------------------------------------------------------------

	std::vector<Dot>& dots = particleEntries.circles;

    //Move all dots
    for (int i = 0; i < PARTICLE_NUM; i++)
    {
        Dot& dot = dots[i];
        dot.movePrediction(TIMEINTERVAL, 0.5f);
    }

    // Update spacial lookup after moving
    updateSpatialLookup(particleEntries);

    //Update densities after moving
    updateDensities(particleEntries);

    for (int i = 0; i < PARTICLE_NUM; i++)
    {
        Dot& dot = dots[i];
        dot.check_wall_no_shift();

        // Update pressure gradient
        pressureGradient = { 0, 0 };

		forParticles(dot, particleEntries, [&](Dot dotB, float magnitude) {
            // Calc pressure gradient (Loop and keep track of pressure gradient)
            
            calculatePressureGradient(pressureGradient, &dotB, &dot);
		});
        
        if (abs(dot.getDensity()) > DENSITY_UPPER)
        {
            dot.addmVelX(pressureGradient[0] / dot.getDensity());
            dot.addmVelY(pressureGradient[1] / dot.getDensity()); 
        }
    }

    ////Move all dots
    for (Dot& dot : dots)
    {
        dot.move(TIMEINTERVAL);
        // COLORING
        int speed = (abs(dot.getVelX()) + abs(dot.getVelY())) * 100;
        std::vector<Uint8> colors = colourProcessor(speed);
        gDotTexture.setColor(colors[0], colors[1], colors[2]);

        // RENDERING
        dot.render(gRenderer, gDotTexture);
    }

    // ========================================================================================================================

    //Update screen
    SDL_RenderPresent(gRenderer);
    interval = timer.getTicks();
}
void engine::runBallCollisionFrame()
{
     // CONTACT METHOD ------------------------------------------------------------------------------------------------------
     for (Dot &dot : dots)
     {
         dot.moveVector(TIMEINTERVAL);
     }

     // Update spacial lookup after moving
     updateSpatialLookup( particleHashEntries, spacialKeys, dots );

     // Check collision for all dots
     for ( int i = 0; i < PARTICLE_NUM; i++)
     {
         Dot &dot = dots[i];
         dot.check_vector_collision( TIMEINTERVAL, wall, dots, particleHashEntries, spacialKeys, i );

         int speed = abs(dot.getVelX()) + abs(dot.getVelY());
         Uint8 colour = 255 - std::min(speed * 10, 255);
         // r = std::rand() % 255;
         // g = std::rand() % 255;
         // b = std::rand() % 255;
         Uint8 r = colour;
         gDotTexture.setColor(r, g, b);
         dot.render( gRenderer, gDotTexture );
     }
     // ========================================================================================================================

}
