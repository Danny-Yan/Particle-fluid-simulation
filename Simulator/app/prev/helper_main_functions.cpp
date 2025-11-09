#include "incl/helper_main_functions.h"

// add namespace helper

void updateDensities(std::vector<Dot> &dots, std::vector<Entry> &particleHashEntries, std::vector<int> &spacialKeys)
{
    float particle_density;
    std::vector<Dot*> filtered_dots;
    for(Dot &dot: dots){
        particleFilter( filtered_dots, dots, particleHashEntries, spacialKeys, dot );
        calculateDensity( particle_density, filtered_dots, dot.getsPosX(), dot.getsPosY() );
        // printf("%f\n", particle_density);
        dot.setDensity( particle_density );
    }
}

void updateDensities(ParticleEntries& particleEntries)
{
	// Unpack particle entries struct
	std::vector<Dot>& dots = particleEntries.circles;

    float particle_density = 0;
    for (Dot& dot : dots) {

        forParticles(dot, particleEntries, [&](Dot& dotB) {
            particle_density += calcDensity(dotB, dot.getsPosX(), dot.getsPosY());
        });

        dot.setDensity(particle_density);

        particle_density = 0;
    }
}

void calculateDensity( float &particle_density, std::vector<Dot*> &circles, float x, float y )
{
    float influence = 0;
    float radius_squared = FORCE_RADIUS * FORCE_RADIUS;

    for (Dot* dotB: circles)
    {
        Circle b = dotB->getColliders();
        float distance_squared = distanceSquared(x, y, b.x, b.y);
        if (distance_squared < radius_squared)
        {
            float distance = sqrt(distance_squared);
            influence += smoothingKernel( distance, FORCE_RADIUS );
        }
    }
    particle_density = influence;
}


float calcDensity(Dot& dotB, float x, float y)
{
    Circle b = dotB.getColliders();
    float influence = 0;
    float distance_squared = distanceSquared(x, y, b.x, b.y);
    if (distance_squared < FORCE_RADIUS_SQUARED)
    {
        float distance = sqrt(distance_squared);
        influence = smoothingKernel(distance, FORCE_RADIUS);
    }

    return influence;
}

float calcDensity(float magnitude)
{
    float influence = 0;
    influence = smoothingKernel(magnitude, FORCE_RADIUS);
    return influence;
}

void calculatePressureGradient( std::vector<float> &pressureGradient, Collider *dotB, Collider *dotA )
{
    Circle a = dotA->getColliders();
    Circle b = dotB->getColliders();
    float normalX = 0;
    float normalY = 0;

    float magnitude = sqrt(distanceSquared(a.x, a.y, b.x, b.y));

    // // CHECK FOR COLLISON
    // if(distance_squared < radius_squared){
    //     dotA->applyDotCollison(*dotB);
    // }

    if (magnitude == 0.0f)
    {
        std::vector<float> randomNormal = getRandomDirection();
        normalX = randomNormal[0];
        normalY = randomNormal[1];
    }
    else 
    {
        normalX = (a.x - b.x) / magnitude;
        normalY = (a.y - b.y) / magnitude;
    }
    float slope = smoothingKernelDerivative(magnitude, FORCE_RADIUS);
    float density = sharedDensity(dotB->getDensity(), dotA->getDensity());
    density = (density != 0.0f) ? density : DENSITY_LOWER;

    pressureGradient[0] += -densityToPressure(density) * normalX * slope / density;
    pressureGradient[1] += -densityToPressure(density) * normalY * slope / density;
}

void calculatePressureGradient(std::vector<float>& pressureGradient, float magnitude, Dot* dotB, Dot* dotA)
{
    Circle a = dotA->getColliders();
    Circle b = dotB->getColliders();
    float normalX;
    float normalY;

    // // CHECK FOR COLLISON
    // if(distance_squared < radius_squared){
    //     dotA->applyDotCollison(*dotB);
    // }

    if (magnitude == 0.0f)
    {
        std::vector<float> randomNormal = getRandomDirection();
        normalX = randomNormal[0];
        normalY = randomNormal[1];
    }
    else
    {
        normalX = (a.x - b.x) / magnitude;
        normalY = (a.y - b.y) / magnitude;
    }
    float slope = smoothingKernelDerivative(magnitude, FORCE_RADIUS);
    float density = sharedDensity(dotB->getDensity(), dotA->getDensity());
    density = (density != 0.0f) ? density : DENSITY_LOWER;

    pressureGradient[0] += -densityToPressure(density) * normalX * slope / density;
    pressureGradient[1] += -densityToPressure(density) * normalY * slope / density;
}

float smoothingKernel( float distance, float radius)
{   
    float volume = M_PI * (pow(radius, 5) / 10);
    float force_factor = radius - distance;
    return force_factor * force_factor * force_factor / volume;
}

float smoothingKernelDerivative( float distance, float radius)
{
    float volume = M_PI * (pow(radius, 5) / 10);
    float force_slope = -3 * (distance - radius) * (distance - radius) / volume;
    return force_slope;
}

std::vector<float> getRandomDirection()
{
    float x = std::rand() % 3 - 1;
    float y = std::rand() % 3 - 1;
    //float magnitude = sqrt(distanceSquared(0, 0, x, y));
    return { x , y };
}

float densityToPressure( float density )
{
    float density_error = density - DENSITY_DESIRED;
    return density_error * FORCE;
}

// Convert to return enumerator
void particleFilter( std::vector<Dot*> &filtered_dots, std::vector<Dot> &circles, std::vector<Entry> &particleHashEntries, std::vector<int> &spacialKeys, Dot &dotA )
{
    filtered_dots.clear();
    //Computing 3x3 spacial hash 
    std::vector<int> spacial_hashes_full = compute_full_spatial_area((int)dotA.getsPosX(), (int)dotA.getsPosY());

    //Iterating through spacial hashes
    for (int hash : spacial_hashes_full)
    {
        int key = spacialKeys[hash];
        for (int i = key; i < circles.size(); i++)
        {
            if (particleHashEntries[i].hash != hash){
                break;
            }

            Dot &dot = circles[particleHashEntries[i].index];
            if ( &dot == &dotA )
            {
                continue;
            }

            filtered_dots.push_back(&dot);
        }
    }
}

// For each loop around particles
void forParticles(Dot& dotA, ParticleEntries& ParticleEntries, const std::function<void(Dot&)>& func)
{
    std::vector<Dot>& dots = ParticleEntries.circles;
    const std::vector<Entry>& particleHashEntries = ParticleEntries.particleHashEntries;
    const std::vector<int>& spacialKeys = ParticleEntries.spacialKeys;

    //Computing 3x3 spacial hash 
    std::vector<int> spacial_hashes_full = compute_full_spatial_area((int)dotA.getsPosX(), (int)dotA.getsPosY());

    //Iterating through spacial hashes
    for (int hash : spacial_hashes_full)
    {
        int key = spacialKeys[hash];
        for (int i = key; i < dots.size(); i++)
        {
            if (particleHashEntries[i].hash != hash) {
                break;
            }

            Dot& dot = dots[particleHashEntries[i].index];

			if (&dot == &dotA)
			{
				continue;
			}

            // Do something
            func(dot);
        }
    }
}

// For each loop around particles
void forParticlesAroundPoint(int x, int y, ParticleEntries& ParticleEntries, const std::function<void(Dot&)>& func)
{
    std::vector<Dot>& dots = ParticleEntries.circles;
    const std::vector<Entry>& particleHashEntries = ParticleEntries.particleHashEntries;
    const std::vector<int>& spacialKeys = ParticleEntries.spacialKeys;

    //Computing 3x3 spacial hash 
    std::vector<int> spacial_hashes_full = compute_full_spatial_area(x, y);

    //Iterating through spacial hashes
    for (int hash : spacial_hashes_full)
    {
        int key = spacialKeys[hash];
        for (int i = key; i < dots.size(); i++)
        {
            if (particleHashEntries[i].hash != hash) {
                break;
            }

            Dot& dot = dots[particleHashEntries[i].index];

            // Do something
            func(dot);
        }
    }
}

// MOUSE HANDLES
void mouseLeftPress( SDL_MouseButtonEvent &b, Mouse *mP){
    Mouse &mouse = *mP;    
    mouse.setIsPressed(true);
    mouse.setForceMultiplier(MOUSE_FORCE);
}

// Right click
void mouseRightPress( SDL_MouseButtonEvent &b, Mouse *mP){
    Mouse &mouse = *mP; 
    mouse.setIsPressed(true);
    mouse.setForceMultiplier(-1 * MOUSE_FORCE);
}

// Left click
void mouseUnPress( SDL_MouseButtonEvent &b, Mouse *mP){
    Mouse &mouse = *mP;    
    mouse.setIsPressed(false);
    mouse.setForceMultiplier(0);
}