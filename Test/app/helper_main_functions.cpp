#include "incl/helper_main_functions.h"

// Adapted from sebastain lague's tutorial
void updateSpatialLookup(std::vector<Entry> &spatialLookup, std::vector<int> &spatialKeys, std::vector<Dot> &dots)
{
    // Compute spatial hash for each dot
    for (int i = 0; i < dots.size(); i++)
    {
        Dot &dot = dots[i];
        int spatialX, spatialY;
        std::tie(spatialX, spatialY) = compute_spatial_coords( (int)dot.getsPosX(), (int)dot.getsPosY() );
        int spatial_hash = compute_spatial_hash( spatialX, spatialY );
        if (spatial_hash < 0) {
            printf("NEGATIVE SPATIAL HASH: %d \n", spatial_hash);
            spatial_hash = abs(spatial_hash);
        }
        spatialLookup[i].hash = spatial_hash;
        spatialLookup[i].index = i;
        spatialKeys[i] = INT_MAX;
    }

    std::sort(spatialLookup.begin(), spatialLookup.end(), [](Entry &a, Entry &b){ return a.hash < b.hash; }); // Sort by hash

    // Compute spatial lookup
    for (int i = 0; i < dots.size(); i++)
    {
        int Key = spatialLookup[i].hash;
        int keyPrev = (i == 0) ? INT_MAX : spatialLookup[i - 1].hash;
        if (Key != keyPrev)
        {
            spatialKeys[Key] = i;
        }
    }
}

// Adapted from sebastain lague's tutorial
void updateSpatialLookup(ParticleEntries& particleEntries)
{
    // Unpack particle entries struct
	std::vector<Entry>& spatialLookup = particleEntries.particleHashEntries; 
    std::vector<Dot>& dots = particleEntries.circles;
	std::vector<int>& spatialKeys = particleEntries.spacialKeys;

    // Compute spatial hash for each dot
    for (int i = 0; i < dots.size(); i++)
    {
        Dot& dot = dots[i];
        int spatialX, spatialY;
        std::tie(spatialX, spatialY) = compute_spatial_coords((int)dot.getsPosX(), (int)dot.getsPosY());
        int spatial_hash = compute_spatial_hash(spatialX, spatialY);
        if (spatial_hash < 0) {
            printf("NEGATIVE SPATIAL HASH: %d \n", spatial_hash);
            spatial_hash = abs(spatial_hash);
        }
        spatialLookup[i].hash = spatial_hash;
        spatialLookup[i].index = i;
        spatialKeys[i] = INT_MAX;
    }

    std::sort(spatialLookup.begin(), spatialLookup.end(), [](Entry& a, Entry& b) { return a.hash < b.hash; }); // Sort by hash

    // Compute spatial lookup
    for (int i = 0; i < dots.size(); i++)
    {
        int Key = spatialLookup[i].hash;
        int keyPrev = (i == 0) ? INT_MAX : spatialLookup[i - 1].hash;
        if (Key != keyPrev)
        {
            spatialKeys[Key] = i;
        }
    }
}


// UPDATE TO MAKE IT WORK BETTER
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

        forParticles(dot, particleEntries, [&](Dot dotB, float mag) {
            particle_density += calcDensity(mag);
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
    float radius_squared = FORCE_RADIUS * FORCE_RADIUS;

    if (distance_squared < radius_squared)
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

void calculatePressureGradient( std::vector<float> &pressureGradient, Dot *dotB, Dot *dotA )
{
    Circle a = dotA->getColliders();
    Circle b = dotB->getColliders();
    float normalX;
    float normalY;

    float magnitude = sqrt(distanceSquared(a.x, a.y, b.x, b.y));

    // // CHECK FOR COLLISON
    // if(distance_squared < radius_squared){
    //     dotA->applyDotCollison(*dotB);
    // }

    if (magnitude == 0)
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

    if (magnitude == 0)
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
// MAKE (circles, particleHashEntries, spacialKeys) STRUCT
void forParticles(Dot& dotA, ParticleEntries& ParticleEntries, const std::function<void(Dot, float)>& func)
{
    std::vector<Dot>& dots = ParticleEntries.circles;
    std::vector<Entry>& particleHashEntries = ParticleEntries.particleHashEntries;
    std::vector<int>& spacialKeys = ParticleEntries.spacialKeys;

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

            // Check if distance is less than radius
            float distance_squared = distanceSquared(dot.getmPosX(), dot.getmPosY(), dotA.getmPosX(), dotA.getmPosY());
            if (&dot == &dotA || distance_squared >= FORCE_RADIUS_SQUARED) {continue;}

            float magnitude = sqrt(distance_squared);

            // Do something
            func(dot, magnitude);
        }
    }
}

// MOUSE HANDLES
void mouseLeftPress( SDL_MouseButtonEvent &b, Mouse *mP){
    Mouse &mouse = *mP;    
    mouse.setForceMultiplier(MOUSE_FORCE);
}

// Right click
void mouseRightPress( SDL_MouseButtonEvent &b, Mouse *mP){
    Mouse &mouse = *mP; 
    mouse.setForceMultiplier(-1 * MOUSE_FORCE);
}

// Left click
void mouseUnPress( SDL_MouseButtonEvent &b, Mouse *mP){
    Mouse &mouse = *mP;    
    mouse.setForceMultiplier(0);
}

// Process speed against rgb values
std::vector<Uint8> colourProcessor(int speed){
    std::vector<Uint8> rgbArr(3);
    rgbArr[0] = colourLinearisation(speed, COLOR_MAX_SPEED, COLOR_MIN[0], COLOR_MAX[0]); // red
    rgbArr[1] = colourLinearisation(speed, COLOR_MAX_SPEED, COLOR_MIN[1], COLOR_MAX[1]); // green
    rgbArr[2] = colourLinearisation(speed, COLOR_MAX_SPEED, COLOR_MIN[2], COLOR_MAX[2]); // blue
    // printf("Speed: %d, R: %d, G: %d, B: %d\n", speed, rgbArr[0], rgbArr[1], rgbArr[2]);
    return rgbArr;
}   

// Linear interpolation of colour against speed
Uint8 colourLinearisation(int speed, int color_max_speed, int color_min, int color_max){
    return (color_max - color_min) * (speed / color_max_speed) + color_min;
}

// // Particle filter iterator using boost
// void particleFilterIterator(coro_t::push_type& yield, std::vector<Dot> &circles, std::vector<Entry> &particleHashEntries, std::vector<int> &spacialKeys, Dot &dotA){
    
//     //Computing 3x3 spacial hash 
//     std::vector<int> spacial_hashes_full = compute_full_spatial_area((int)dotA.getPosX(), (int)dotA.getPosY());

//     //Iterating through spacial hashes
//     for (int hash : spacial_hashes_full)
//     {
//         int key = spacialKeys[hash];
//         if ( key == INT_MAX){
//             continue;
//         }
//         for (int i = key; i < circles.size(); i++)
//         {
//             if (particleHashEntries[i].hash != hash){
//                 break;
//             }

//             Dot &dot = circles[particleHashEntries[i].index];
//             if ( &dot == &dotA )
//             {
//                 continue;
//             }
//             yield(&dot);
//         }
//     }
// }