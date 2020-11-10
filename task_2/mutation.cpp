#include <iostream>
#include <random>
#include <chrono>

#include "mutation.h"
#include "solution.h"


AbstractSolution* Mutation::modifySolution(AbstractSolution* solut) {
    //
    AbstractSolution *init_solution = solut->copy_solution();

    if (init_solution->scheduling.size() <= 1) return init_solution;

    int proc_1 = arc4random_uniform(init_solution->scheduling.size());
    auto size = init_solution->scheduling[proc_1].size();
    while (size == 0)
    {
        proc_1 = arc4random_uniform(init_solution->scheduling.size());
        size = init_solution->scheduling[proc_1].size();
    }

    int task = arc4random_uniform(size);
    int proc_2 = arc4random_uniform(init_solution->scheduling.size());

    while (proc_1 == proc_2){
        proc_2 = arc4random_uniform(init_solution->scheduling.size());
    }

    int left =0;
    int right = init_solution->scheduling[proc_2].size();
    int execution_time = (*init_solution->tasks)[init_solution->scheduling[proc_1][task]];
    int middle = (left + right) / 2;
    while ((right - left) > 1){
        int ind = init_solution->scheduling[proc_2][middle];
        if ((*init_solution->tasks)[ind] > execution_time){
            right = middle;
        }
        else if ((*init_solution->tasks)[ind] > execution_time) {
            left = middle;
        }
        else break;
        middle = (right + left) / 2;
    }   

    int id = init_solution->scheduling[proc_1][task];
    if (init_solution->scheduling[proc_2].size() == 0) {
        init_solution->scheduling[proc_2].push_back(id);
        init_solution->scheduling[proc_1].erase(init_solution->scheduling[proc_1].begin() + task);
        return init_solution;
    }

    int execution_time_to = (*init_solution->tasks)[id];
    if (execution_time < execution_time_to) {
        init_solution->scheduling[proc_2].insert(init_solution->scheduling[proc_2].begin(), id);
    }
    else
    {
        init_solution->scheduling[proc_2].insert(init_solution->scheduling[proc_2].begin() + middle + 1, id);
    }
    init_solution->scheduling[proc_1].erase(init_solution->scheduling[proc_1].begin() + task);
    init_solution->update();
    return init_solution;

}
   