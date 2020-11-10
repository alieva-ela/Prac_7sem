#ifndef STARTIO_H
#define STARTIO_H

#include "solution.h"
#include "mutation.h"
#include "temperature.h"

class StartIO {
private:
    AbstractSolution *curSolution = nullptr;
    AbstractSolution *bestSolution = nullptr;
    AbstractMutation *mut = nullptr;
    Temp *temp = nullptr;
    int iterations = 1;
public:
	//StartIO() = delete;
    StartIO(AbstractSolution *initialSolution, AbstractMutation *initialMut, Temp *initialTemp) :
        curSolution(initialSolution), bestSolution(curSolution->copy_solution()), mut(initialMut), temp(initialTemp) 
    {}


    double P(const double& dE, const double& t) const {
        return std::exp(-dE / t);
    }

    AbstractSolution* mainAlgorithm() {
        std::mt19937 engine;
        engine.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
        std::uniform_real_distribution<double> dist(0, 1);

        const size_t MAX_ITERATION_OUT = 4000000000;

        size_t lastUpdateSolution = 0;

        while (lastUpdateSolution < 100 && iterations < MAX_ITERATION_OUT) {
            auto newSolution = mut->modifySolution(curSolution);


            double dE = newSolution->res - bestSolution->res;


            if (dE <= 0 || (dist(engine) < P(dE, temp->getDecreaseTemp(iterations)))) {
                delete curSolution;
                curSolution = newSolution;
            }
            else {
                delete newSolution;
            }
            
            if (curSolution->res < bestSolution->res) {
                delete bestSolution;
                bestSolution = curSolution->copy_solution();
                lastUpdateSolution = 0;
            }
            lastUpdateSolution++;
            iterations++;
        }
        return bestSolution;
    }


    AbstractSolution* mainAlgorithm_revers() {
        std::mt19937 engine;
        engine.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
        std::uniform_real_distribution<double> dist(0, 1);

        const size_t MAX_ITERATION_OUT = 4000000000;

        size_t lastUpdateSolution = 0;

        while (lastUpdateSolution < 100 && iterations < MAX_ITERATION_OUT) {
            auto newSolution = mut->modifySolution(curSolution);


            double dE = bestSolution->res - newSolution->res;


            if (dE >= 0 || (dist(engine) < P(dE, temp->getDecreaseTemp(iterations)))) {
                delete curSolution;
                curSolution = newSolution;
            }
            else {
                delete newSolution;
            }
            
            if (curSolution->res > bestSolution->res) {
                delete bestSolution;
                bestSolution = curSolution->copy_solution();
                lastUpdateSolution = 0;
            }
            lastUpdateSolution++;
            iterations++;
        }
        return bestSolution;
    }

    // ~StartIO() = default;
};

#endif