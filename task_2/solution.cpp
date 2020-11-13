// #include "task.h"
#include "solution.h"

#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>

AbstractSolution::AbstractSolution(){}

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

    AbstractSolution *new_solution = new Solution();
new_solution->tasks = new std::vector<uint64_t>();
new_solution->scheduling = scheduling;
*new_solution->tasks = *tasks;
new_solution->res = res;
new_solution->number_of_processors = number_of_processors;
return new_solution;
}

// void Solution::sort_task(){
//     for (int i = 0; i < scheduling.size(); i++) {
//         std::sort(scheduling[i].begin(), scheduling[i].end(), [&](int i1, int i2)
//                                                               {
//                                                                     return (*tasks)[i1] < (*tasks)[i2];
//                                                               })
//     }
//     // for (int r = 0; r < t.size(); r++) { 
//     //     if (t[r].size()==0){
//     //         continue;
//     //     }
//     //     for (int i = 0; i < t[r].size() - 1; i++) {
//     //         for (int j = 0; j < t[r].size() - i - 1; j++) {
//     //             if (p[t[r][j]]->get_processing_time() > p[t[r][j + 1]]->get_processing_time()) {
//     //                 temp = t[r][j];
//     //                 t[r][j] = t[r][j + 1];
//     //                 t[r][j + 1] = temp;
//     //            }
//     //         }
//     //     }
//     // }
//     // scheduling.resize(solut->get_num_procs());
//     // scheduling = t;
// }

// void Solution::solution_init() {
//     // std::vector<std::vector<int>> task;
//     scheduling.resize(number_of_processors);
//     // std::mt19937 engine;
//     // engine.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
//     // std::uniform_int_distribution<size_t> dist(0, scheduling.size() - 1);

//     int rand;
//     for (int i = 0; i < tasks->size(); i++) {
//         rand = arc4random_uniform(number_of_processors);
//         scheduling[rand].push_back(i);
//     }
//     std::cout<<"init"<<std::endl;
//     //print();
//     sort_task();
//     std::cout<<"sort"<<std::endl;
//     print();
//     solution_sum();
// }

// void Solution::solution_sum() {
//     res = 0;
//     for (size_t i = 0; i < scheduling.size(); i++) {
//         int start = 0;
//         for (auto& j : scheduling[i]) {
//             res += start += (*tasks)[j];
//         }
//     }
// }

void Solution::update() {
    res = 0;
    for (size_t i = 0; i < scheduling.size(); i++) {
        int start = 0;
        for (auto& j : scheduling[i]) {
            res += start += (*tasks)[j];
        }
    }
}

// void Solution::update() {
//     //SolutionScheduling* sol = dynamic_cast<SolutionScheduling*>(s);
//     // Solution *init_solution = new Solution(tasks);
//     // if (init_solution == this) {
//     //     return;
//     // }

//     res = 0;
//     for (int i = 0; i < scheduling.size(); i++) {
//         // scheduling[i].clear();
//         for (auto& j : init_solution->scheduling[i]) {
//             scheduling[i].push_back(j);
//         }
//     }
//     set_res(init_solution->get_res());
// }


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
    for (int i = 0; i < scheduling.size(); i++) {
        std::sort(scheduling[i].begin(), scheduling[i].end(), [&](int i1, int i2)
                                                              {
                                                                    return (*tasks)[i1] < (*tasks)[i2];
                                                              });
    }
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
