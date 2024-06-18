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