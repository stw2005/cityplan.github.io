#ifndef DISJOINT_SET_HPP
#define DISJOINT_SET_HPP

#include <iostream>
#include <vector>
#include "custom-exceptions.hpp"

#define INDEX unsigned int

/*
    Implements a disjoint set which follows the principle of the Disjoint Set 
    Union (DSU) or UNION-FIND
*/
class DisjointSet {
private:
    std::vector<INDEX> container;
    unsigned int size;

public:
    // Constructor
    DisjointSet(unsigned int);

    // Checks if the root of the given sets is the same.
    bool FIND(INDEX, INDEX);

    // Combines two sets.
    void UNION(INDEX, INDEX);
};

DisjointSet::DisjointSet(unsigned int cardinality): size(cardinality), container(std::vector<INDEX>(cardinality)) {
    for (int i = 0; i < size; i++)
        container[i] = i;
}

bool DisjointSet::FIND(INDEX a, INDEX b) {
    try {
        if (a < size and b < size)
            return container[a] == container[b];
        else
            throw OutOfBoundException();
    } catch(const OutOfBoundException& e) {
        std::cerr << e.what() << '\n';
    }
    return false;
}

void DisjointSet::UNION(INDEX a, INDEX b) {
    try {
        if (a < size and b < size) {
            int temp = container[a];
            for (int i = 0; i < size; i++) {
                if (container[i] == temp)
                    container[i] = container[b];
            }
        } else
            throw OutOfBoundException();
    } catch(const OutOfBoundException& e) {
        std::cerr << e.what() << '\n';
    }
}

#endif
