#ifndef MUTATION_H
#define MUTATION_H

// #include <iostream>

// #include "task.h"
#include "solution.h"

class AbstractMutation {
public:
    AbstractMutation() = default;

    virtual AbstractSolution* modifySolution(AbstractSolution* solution) = 0;

    virtual ~AbstractMutation() = default;
};

class Mutation : public AbstractMutation {
public:
    Mutation() = default;

    virtual AbstractSolution* modifySolution(AbstractSolution* solution) override; 

    // virtual ~Mutation() = default;
};


#endif