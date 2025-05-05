#ifndef HELPER_STRUCTS_H
#define HELPER_STRUCTS_H
#include <vector>

class Dot;

struct Entry{
    int hash;
    int index;
    Entry(){}
    Entry(int hash, int index){
        this->hash = hash;
        this->index = index;
    }
};



struct ParticleEntries{

    // CIRCLES NOT INITIALISED TO THE SAME LENGTH
    std::vector<Dot> circles;
    std::vector<Entry> particleHashEntries = std::vector<Entry>(PARTICLE_NUM);
    std::vector<int> spacialKeys = std::vector<int>(PARTICLE_NUM);

	ParticleEntries() {}
	//ParticleEntries(std::vector<Dot> *circles, std::vector<Entry> *particleHashEntries, std::vector<int> *spacialKeys) {
	//	this->circles = circles;
	//	this->particleHashEntries = particleHashEntries;
	//	this->spacialKeys = spacialKeys;
	//}

	~ParticleEntries() {
		circles.clear();
		particleHashEntries.clear();
		spacialKeys.clear();
	}
};

#endif