#include "helper_main_functions.h"

// Adapted from sebastain lague's tutorial
void updateSpatialLookup(std::vector<Entry> &spatialLookup, std::vector<int> &spatialKeys, std::vector<Dot> &dots){

    // Compute spatial hash for each dot
    for (int i = 0; i < dots.size(); i++)
    {
        Dot &dot = dots[i];
        int spatialX, spatialY;
        std::tie(spatialX, spatialY) = compute_spatial_coords( (int)dot.getmPosX(), (int)dot.getmPosY() );
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
        int keyPrev;
        if (i == 0){keyPrev = INT_MAX;} else {keyPrev = spatialLookup[i - 1].hash;}
        if (Key != keyPrev){
            spatialKeys[Key] = i;
        }
    }
}

float calculateDensity( std::vector<Dot*> &circles, Dot &dot )
{
    int amount = 0;
    Circle a = dot.getColliders();
    float radius_squared = FORCE_RADIUS * FORCE_RADIUS;

    for (Dot* dotB: circles)
    {
        if (&dot == dotB)
        {
            continue;
        }
        Circle b = dotB->getColliders();
        float distance_squared = distanceSquared(a.x, a.y, b.x, b.y);
        if (distance_squared < radius_squared)
        {
            amount++;
        }
    }
    float area = (a.r * a.r);

    float density = amount / area;
    return density;
}

void particleFilter( std::vector<Dot*> &Dots, std::vector<Dot> &circles, std::vector<Entry> &particleHashEntries, std::vector<int> &spacialKeys, Dot &dotA )
{
    Dots.clear();
    //Computing 3x3 spacial hash 
    std::vector<int> spacial_hashes_full = compute_full_spatial_area((int)dotA.getmPosX(), (int)dotA.getmPosY());

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
            if (&dot == &dotA)
            {
                continue;
            }

            Dots.push_back(&dot);
        }
    }
}