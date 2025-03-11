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
        Entry entry = Entry(spatial_hash, i);
        spatialLookup[i] = entry;
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

std::vector<float> calculatePressureGradient( std::vector<float> &pressureGradient, Dot *dotB, Dot *dotA )
{
    Circle a = dotA->getColliders();
    float radius_squared = FORCE_RADIUS * FORCE_RADIUS;

    Circle b = dotB->getColliders();
    float normalX;
    float normalY;

    float distance_squared = distanceSquared(a.x, a.y, b.x, b.y);
    if (distance_squared < radius_squared)
    {
        float magnitude = sqrt(distance_squared);
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
        float density = sharedDensity( dotB->getDensity(), dotA->getDensity() );
        pressureGradient[0] += -densityToPressure(density) * normalX * slope / density;
        pressureGradient[1] += -densityToPressure(density) * normalY * slope / density;
    }

    return pressureGradient;
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
    int x = std::rand() % 50 - 25;
    int y = std::rand() % 50 - 25;
    float magnitude = sqrt(distanceSquared(0, 0, x, y));
    return { x / magnitude, y / magnitude };
}

float densityToPressure( float density )
{
    float density_error = density - DENSITY_DESIRED;
    return density_error * FORCE;
}



// Convert to iterator
void particleFilter( std::vector<Dot*> &filtered_dots, std::vector<Dot> &circles, std::vector<Entry> &particleHashEntries, std::vector<int> &spacialKeys, Dot &dotA )
{
    filtered_dots.clear();
    //Computing 3x3 spacial hash 
    std::vector<int> spacial_hashes_full = compute_full_spatial_area((int)dotA.getsPosX(), (int)dotA.getsPosY());

    //Iterating through spacial hashes
    for (int hash : spacial_hashes_full)
    {
        int key = spacialKeys[hash];
        if ( key == INT_MAX){
            continue;
        }
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

// MOUSE HANDLES

void mousePress( SDL_MouseButtonEvent &b, Mouse *mP){
    Mouse &mouse = *mP;    
    if (b.button == SDL_BUTTON_LEFT){
        // for (Dot* dot: dots){
        //     Dot &dotA = *dot;
        //     //COLLISION CHECK
        //     Collision cLVector = dotA.checkCircleForce( mouse );
        //     if (cLVector.didCollide)
        //     {
        //         //Momentum transfer
        //         dotA.addmVelX( cLVector.v[0] * mouse.getForceMultiplier() );
        //         dotA.addmVelY( cLVector.v[1] * mouse.getForceMultiplier() );
        //     }
        // }
        mouse.setForceMultiplier(MOUSE_FORCE);
    }
}

void mouseUnPress( SDL_MouseButtonEvent &b, Mouse *mP){
    Mouse &mouse = *mP;    
    mouse.setForceMultiplier(0);
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