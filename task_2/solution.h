#ifndef SOLUTION_H
#define SOLUTION_H

#include <iostream>
#include <vector>

// #include "task.h"


class AbstractSolution{
// private:
//     int res;
public:
    int res;
    int number_of_processors;
    std::vector<std::vector<int>> scheduling;
    std::vector<uint64_t>* tasks;

    AbstractSolution(std::vector<uint64_t>* sol, int number_of_processors);
    virtual void print() = 0;
    virtual AbstractSolution* copy_solution() = 0;
    // virtual void sort_task() =0;
    // virtual void solution_init() = 0;
    // virtual void solution_sum() = 0;

   virtual void update() = 0;

    // double get_res() const {
    //     return res;
    // }
    // void set_res(const int& r) {
    //         res = r;
    // }
    virtual ~AbstractSolution() { delete tasks; }
};

class Solution : public AbstractSolution {

   // Schedule* solut;

public:

   // Solution() = delete;

    Solution(std::vector<uint64_t>* sol, int number_of_processors) 
    : AbstractSolution(sol, number_of_processors) {}

    virtual void print() override;
    virtual AbstractSolution* copy_solution() override;
    // virtual void sort_task() override;
    // virtual void solution_init() override;
    // virtual void solution_sum() override;
    virtual void update() override;
    // virtual ~Solution() { delete tasks; }

};

#endif