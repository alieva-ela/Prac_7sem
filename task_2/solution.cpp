// #include "task.h"
#include "solution.h"

#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>

void Solution::print(){
    std::cout << "number_of_processors = " << number_of_processors << std::endl;
    std::cout << "res = " << res << std::endl;
    for (int i = 0; i < scheduling.size(); i++) { 
        for ( 
            auto it = scheduling[i].begin(); 
            it != scheduling[i].end(); it++) 
            std::cout << *it << " "; 
        std::cout << std::endl; 
    } 
}

AbstractSolution *Solution::copy_solution() {

    AbstractSolution *new_solution = new Solution(tasks, number_of_processors);
    new_solution->scheduling = scheduling;
    *new_solution->tasks = *tasks;
    new_solution->res = res;
    new_solution->number_of_processors = number_of_processors;
    return new_solution;
}


void Solution::update() {
    res = 0;
    for (size_t i = 0; i < scheduling.size(); i++) {
        int start = 0;
        for (auto& j : scheduling[i]) {
            res += start += (*tasks)[j];
        }
    }
}

AbstractSolution::AbstractSolution(std::vector<uint64_t>* tasks, int  number_of_processors) {
    //scheduling. sol->scheduling.size();
    this->number_of_processors =  number_of_processors;
    this->tasks = new std::vector<uint64_t>(tasks->size());
    for (int i = 0; i < tasks->size(); i++) (*this->tasks)[i] = (*tasks)[i];
    // (*(this->tasks)) = (*tasks);
    // solut = sol;
    // solution_init();
    scheduling.resize(number_of_processors);

    int rand;
    for (int i = 0; i < tasks->size(); i++) {
        rand = arc4random_uniform(number_of_processors);
        scheduling[rand].push_back(i);
    }
    // std::cout<<"init"<<std::endl;
    /*for (int i = 0; i < scheduling.size(); i++) {
        std::sort(scheduling[i].begin(), scheduling[i].end(), [&](int i1, int i2)
                                                              {
                                                                    return (*tasks)[i1] < (*tasks)[i2];
                                                              });
    }*/
    // std::cout<<"sort"<<std::endl;
    res = 0;
    for (size_t i = 0; i < scheduling.size(); i++) {
        int start = 0;
        for (auto& j : scheduling[i]) {
            res += start += (*tasks)[j];
        }
    }

    // std::cout << "<Solution>\n";
}
