#ifndef TASK_H
#define TASK_H
#include <vector>

// class Task {
//         int processing_time;
//         int processor;
//         const int id;
// public:
 
//     Task(int processing_time, int processor, int id) :
//     processing_time(processing_time), processor(processor), id(id) {
//     }

//     int get_id() {
//         return id;
//     }

//     int get_proc_index() {
//     return processor;
//     }

//     int get_processing_time() {
//         return processing_time;
//     }

//     Task* copy_problem() {
//         Task *new_task = new Task(this->processing_time, this->processor, this->id);
    
//     return new_task;
//     }

//     ~Task();

// };


class Schedule{
    //std::vector<IO*> io_tasks;
    //std::vector<int> order;
    // int number_of_processors;

public:
    double init_temp;
    std::vector<uint64_t> *task;
    
    Schedule(double init_temp = 1000.0, 
            std::vector<uint64_t> *task = nullptr) 
            :  init_temp(init_temp), task(task) {
    }

    // std::vector<Task *> *get_task() {
    //     return task;
    // }

    // size_t size() const {
    //     return task->size();
    // }

    // int get_num_procs(){
    //     return number_of_processors;
    // }

    ~Schedule(){
        for (auto now : *this->task) {
            delete now;
        }
    }
    
};

#endif