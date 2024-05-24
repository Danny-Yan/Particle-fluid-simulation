#include "helper_main_functions.h"

// Adapted from sebastain lague's tutorial
void updateSpacialLookup(std::vector<Entry> &spacialLookup, std::vector<int> &spacialKeys, std::vector<Dot> &dots){

    // Compute spatial hash for each dot
    for (int i = 0; i < dots.size(); i++)
    {
        Dot &dot = dots[i];
        dot.compute_spacial_coords();
        int spacial_hash = dot.compute_spacial_hash(dot.getSpacialX(), dot.getSpacialY());
        Entry entry = Entry(spacial_hash, i);
        spacialLookup[i] = entry;
        spacialKeys[i] = INT_MAX;
    }

    std::sort(spacialLookup.begin(), spacialLookup.end(), [](Entry &a, Entry &b){ return a.hash < b.hash; }); // Sort by hash

    // Compute spacial lookup
    for (int i = 0; i < dots.size(); i++)
    {
        int Key = spacialLookup[i].hash;
        int keyPrev;
        if (i == 0){keyPrev = INT_MAX;} else {keyPrev = spacialLookup[i - 1].hash;}
        if (Key != keyPrev){
            spacialKeys[Key] = i;
        }
    }
}