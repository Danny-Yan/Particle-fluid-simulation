#include "../incl/engine.h"
#include "../../images/transparent_dot.xpm"


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
    gwindow = SDL_CreateWindow("Fluid Simulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

    ////Load Foo' texture
    //if (!gDotTexture.loadFromFile(gRenderer, "images/transparent_dot.png"))
    //{
    //    printf("Failed to load dot' texture image!\n");
    //    success = false;
    //}

    //Load Foo' texture
    if (!gDotTexture.loadFromXPM(gRenderer, const_cast<char**>(transparent_dot_xpm)))
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
bool engine::start() {
    //Start up SDL and create window
    if (!initSDL())
    {
        printf("Failed to initialize!\n");
        return false;
    }

    //Load media
    if (!loadMedia())
    {
        printf("Failed to load media!\n");
        system("pause");
        return false;
    }

    timer.start();

    return true;
}

bool engine::isRunning() {
    return !quit;
}

void engine::run()
{
    if (!start()) {
        return;
    }
    // Setup simulation
    mGeneralSimSetUp();

    // Run simulation
	whileRunning([&](){ mRunFluidSimFrame();});

	// Close simulation when done
    close();
}

void engine::mGeneralSimSetUp() {
	// Create particles
    particleManager.createParticles([&]() {
        float spacing_scale = RADIUS * SCALE + SPACING;
        int particlesPerRow = (int)sqrt(PARTICLE_NUM);
        int particlesPerCol = (PARTICLE_NUM - 1) / particlesPerRow + 1;
        float x_cord;
        float y_cord;

        for (int i = 0; i < PARTICLE_NUM; i++)
        {
            // radius = std::rand() % 10 + 5;
            float x = (i % particlesPerRow - particlesPerRow / 2.0f + 0.5f) * (spacing_scale);
            float y = (i / particlesPerRow - particlesPerCol / 2.0f + 0.5f) * (spacing_scale);

            x_cord = x + PARTICLE_START_X;
            y_cord = y + PARTICLE_START_Y;
            particleManager.getDots().emplace_back(x_cord, y_cord, 0, 0, RADIUS);
        }
    });

    particleManager.updateDensities();
}

void engine::whileRunning(const std::function<void()>& func)
{
    while (isRunning()) {
        pollEvent();
        clearScreen();

        ////Render wall
        //SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
        //SDL_RenderDrawRect(gRenderer, &wall);

        if (timer.timerHasTicked(TIMEINTERVAL) && timer.isRunning() == true)
        {
           func();
           deltaTime = 0;
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
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
			mouse.setmPosX((float)mouseX);
			mouse.setmPosY((float)mouseY);
            mouse.move();
            break;

            // MOUSE CLICK
        case SDL_MOUSEBUTTONDOWN:
            switch (e.button.button) {
            case SDL_BUTTON_LEFT:
                Helper::mouseLeftPress(e.button, &mouse);
                break;
            case SDL_BUTTON_RIGHT:
                Helper::mouseRightPress(e.button, &mouse);
                break;

            }
            break;

        case SDL_MOUSEBUTTONUP:
            Helper::mouseUnPress(e.button, &mouse);
            break;

            // KEYBOARD
        case SDL_KEYDOWN:
            switch (e.key.keysym.sym) {

                // Start/stop
            case SDLK_s:
                switch (timer.isRunning()) {
				    case true:
					    timer.stop();
					    break;
				    case false:
					    timer.start();
					    break;
                }
                break;

                // Pause/Unpause
            case SDLK_p:
                break;

                // Ticked
            case SDLK_t:
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

void engine::mRunFluidSimFrame()
{
    //DENSITY METHOD w/ PREDICTIVE STEPS -------------------------------------------------------------------------------------------------------

	std::vector<Dot>& dots = particleManager.getDots();

    //Move all dots
    for (Dot& dot : dots)
    {
        dot.movePrediction(TIMEINTERVAL, PREDICTIVE_STEPS);
    }

    // Update spacial lookup after moving
    particleManager.updateSpatialLookup();

    //Update densities after moving
    particleManager.updateDensities();

    // Run simulation frames
    runFluidParticlesFrame();
    runFluidMouseForceFrame();
    runSimRenderFrame(TIMEINTERVAL);

    //Update screen
    SDL_RenderPresent(gRenderer);
}

void engine::runBallCollisionFrame()
{
     // CONTACT METHOD ------------------------------------------------------------------------------------------------------
     //for (Dot &dot : dots)
     //{
     //    dot.moveVector(TIMEINTERVAL);
     //}

     //// Update spacial lookup after moving
     //updateSpatialLookup( particleHashEntries, spacialKeys, dots );

     //// Check collision for all dots
     //for ( int i = 0; i < PARTICLE_NUM; i++)
     //{
     //    Dot &dot = dots[i];
     //    dot.check_vector_collision( TIMEINTERVAL, wall, dots, particleHashEntries, spacialKeys, i );

     //    int speed = abs(dot.getVelX()) + abs(dot.getVelY());
     //    Uint8 colour = 255 - std::min(speed * 10, 255);
     //    // r = std::rand() % 255;
     //    // g = std::rand() % 255;
     //    // b = std::rand() % 255;
     //    Uint8 r = colour;
     //    gDotTexture.setColor(r, g, b);
     //    dot.render( gRenderer, gDotTexture );
     //}
     // ========================================================================================================================
}

// ------------------------------------------------------------------------------------------

void engine::runFluidParticlesFrame()
{
	std::vector<Dot>& dots = particleManager.getDots();
    // Loop through and update all particles
    for (Dot &dot: dots)
    {
        // Update pressure gradient
        pressureGradient = { 0, 0 };

        particleManager.forParticles(dot, [&](Dot& dotB) {
            // Check if distance is less than radius
            float distance_squared = Helper::distanceSquared(dot.getmPosX(), dot.getmPosY(), dotB.getmPosX(), dotB.getmPosY());

			
   //         if (distance_squared < RADIUS_SQUARED) {
			//	// Calc pressure gradient (Loop and keep track of pressure gradient)
			//	//particleManager.calculatePressureGradient(pressureGradient, &dotB, &dot);
			//}
   //         else 
            if (distance_squared < FORCE_RADIUS_SQUARED) {
                // Calc pressure gradient (Loop and keep track of pressure gradient)
                particleManager.calculatePressureGradient(pressureGradient, &dotB, &dot);
            }
            //dot.applyDotCollison(dotB);
        });

        dot.check_wall_no_shift();
        dot.check_wall_collision();

        if (abs(dot.getDensity()) > DENSITY_UPPER)
        {
            dot.addmVelX(pressureGradient[0] / dot.getDensity());
            dot.addmVelY(pressureGradient[1] / dot.getDensity());
        }
    }
}
void engine::runFluidMouseForceFrame()
{
    if (mouse.hasBeenPressed() == false) { return; }
    // Loop through area around mouse cursor and update
    particleManager.forParticlesAroundPoint(mouse.getmPosX(), mouse.getmPosY(), [&](Dot& dotB) {
        // Check if distance is less than radius
        float distance_squared = Helper::distanceSquared(mouse.getmPosX(), mouse.getmPosY(), dotB.getmPosX(), dotB.getmPosY());

        if (distance_squared < MOUSE_RADIUS_SQUARED) {
            //printf("distance_squared: %f\n", distance_squared);
            // Calc and apply mouse force
            dotB.check_mouse_force(&mouse);
        }
    });
}
void engine::runSimRenderFrame(int timeInterval)
{
	std::vector<Dot>& dots = particleManager.getDots();
    ////Move all dots
    float xTotal = 0;
    float yTotal = 0;
    for (Dot& dot : dots)
    {
        dot.move(timeInterval);
        // COLORING
        float speed = (abs(dot.getVelX()) + abs(dot.getVelY()));
        gDotTexture.setColorForSpeedHSL(speed);

        // RENDERING
		xTotal += dot.getVelX();
        yTotal += dot.getVelY();
        dot.render(gRenderer, gDotTexture);
    }

	printf("total change %f %f\n", xTotal, yTotal);
}
void engine::runFluidMouseDensityFrame()
{
    if (mouse.hasBeenPressed() == false) { return; }
    // Loop through area around mouse cursor and update
    particleManager.forParticlesAroundPoint(mouse.getmPosX(), mouse.getmPosY(), [&](Dot& dotB) {
        // Check if distance is less than radius
        float distance_squared = Helper::distanceSquared(mouse.getmPosX(), mouse.getmPosX(), dotB.getmPosX(), dotB.getmPosY());

        pressureGradient = { 0, 0 };
        if (distance_squared < MOUSE_RADIUS_SQUARED) {
            //printf("distance_squared: %f\n", distance_squared);
            // Calc and apply mouse force
            particleManager.calculatePressureGradient(pressureGradient, &dotB, &mouse);
            //printf("%f %f\n", pressureGradient[0], pressureGradient[1]);
        }

        if (abs(dotB.getDensity()) > DENSITY_UPPER)
        {
            dotB.addmVelX(pressureGradient[0] / dotB.getDensity());
            dotB.addmVelY(pressureGradient[1] / dotB.getDensity());
        }
    });
}
