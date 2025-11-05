#pragma once

#include <vector>

class Dot;

struct Entry{
    int hash = 0;
    int index = 0;
    Entry(){}
    Entry(int hash, int index){
        this->hash = hash;
        this->index = index;
    }
};

//A circle stucture
struct Circle
{
    int x, y;
    int r;
};

//A collision vector
struct Collision
{
    std::vector<float> v;
    std::vector<float> sa;
    bool didCollide = false;
};