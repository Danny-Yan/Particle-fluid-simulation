#ifndef HELPER_STRUCTS_H
#define HELPER_STRUCTS_H

struct Entry{
    int hash;
    int index;
    Entry(){}
    Entry(int hash, int index){
        this->hash = hash;
        this->index = index;
    }
};

#endif