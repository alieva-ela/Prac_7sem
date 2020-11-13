
#include <cstring>
#include <iostream>
#include <random>
#include <vector>
#include <chrono>
#include <thread>
#include <unistd.h>

#include "solution.h"
#include "mutation.h"
#include "tinyxml2.h"
#include "temperature.h"
#include "startio.h"


// void load_file_parseDate(std::vector<uint64_t>* tasks, int& num_proc, char* fileName) {

//     TiXmlDocument doc( fileName );
//     if (!doc.LoadFile()) return;

//     TiXmlHandle hDoc(&doc);
//     TiXmlElement* pElem;
//     TiXmlHandle hRoot(0);

//     pElem=hDoc.FirstChildElement().Element();
//     if (!pElem) return;

//     hRoot=TiXmlHandle(pElem);

//     pElem=hRoot.FirstChild( "Processor" ).Element();
//     pElem->QueryIntAttribute("processsor", &num_proc);

//     hRoot1=TiXmlHandle(pElem);

//     pElem1=hRoot1.FirstChild( "Jobs" ).Element();
//     int num_tasks;
//     pElem1->QueryIntAttribute("jobs", &num_tasks);
//     //std::cout << schedule.number_of_processors<<std::endl;
//     tasks->resize(num_tasks);

//     pElem2=hRoot1.FirstChild( "Job" ).Element();
//     while (pElem2 != nullptr)
//     {
//         int id, execution_time;
//         pElem2->QueryIntAttribute("job", &id);
//         pElem2->QueryIntAttribute("time", &execution_time);
//         (*tasks)[id] = execution_time;
//         pElem2 = pElem2->NextSiblingElement("Job").Element();
//     }
// }

std::vector<uint64_t>* load_file_parseDate(std::vector<uint64_t>* tasks, int& num_proc, char* fileName) {

    using namespace tinyxml2;

    XMLError eResult;
    XMLDocument xmlDocument;
    eResult = xmlDocument.LoadFile(fileName); // load XML file
    //XMLCheckResult(eResult);
    XMLNode * xmlNode = xmlDocument.FirstChildElement(); // root tag
    if (xmlNode == nullptr) exit(XML_ERROR_FILE_READ_ERROR);

    XMLElement * pListElement = xmlNode->FirstChildElement("Processor");
    if (pListElement == nullptr) exit(XML_ERROR_PARSING_ELEMENT);

    eResult = pListElement->QueryIntAttribute("processsor", &num_proc);
    // XMLCheckResult(eResult);

    int num_tasks;
    pListElement = xmlNode->FirstChildElement("Jobs");
    eResult = pListElement->QueryIntAttribute("jobs", &num_tasks);
    tasks = new std::vector<uint64_t>(num_tasks);
   
    //int randp = arc4random_uniform(num_proc);
    pListElement = xmlNode->FirstChildElement("Job");
    while (pListElement != nullptr)
    {
        int id, executionTime;
        eResult = pListElement->QueryIntAttribute("job", &id);
        eResult = pListElement->QueryIntAttribute("time", 
                                                  &executionTime);

        (*tasks)[id] = executionTime;
        pListElement = pListElement->NextSiblingElement("Job");
    }

    return tasks;
}

void init(AbstractSolution *solution, AbstractMutation *mutation, int i, AbstractSolution **Res, int thr){
    if (i == 1){
        StartIO<TempFirst> IO(solution, mutation, 1000);
        *Res = IO.mainAlgorithm();
    }else if(i == 2){
        StartIO<TempSecond> IO(solution, mutation, 1000);
        *Res = IO.mainAlgorithm();
    }else{
        StartIO<TempThird> IO(solution, mutation, 1000);
        *Res = IO.mainAlgorithm();
    }
    /*StartIO<temp> IO(solution, mutation, 1000);
    *Res = IO.mainAlgorithm();*/
}

int main(int argc, char* argv[]) {

    //const double INIT_TEMPERATURE = 10000;
    size_t numThreads = sysconf(_SC_NPROCESSORS_ONLN);
    if (argc == 4) {
        numThreads = std::atoi(argv[3]);
    }


    int num_proc;
    std::vector<uint64_t>* tasks = nullptr;// = new std::vector<uint64_t>(20);
    
    tasks = load_file_parseDate(tasks, num_proc, argv[1]);

    if (numThreads <= 1) {
        std::cout << "Start"<< std::endl;
        std::cout << "TASK VECTOR: " << std::endl;
        for (int i = 0; i < tasks->size(); i++)
        {
            std::cout << "ID = " << i << " PROCESSING TIME = " << (*tasks)[i] << " \n";
        }

        std::cout << std::endl;
        auto begin = std::chrono::steady_clock::now();
        AbstractSolution* solution = new Solution(tasks, num_proc);
        solution->print();
        AbstractMutation* mutation = new Mutation;
        //Temp *temp = new TempSecond(1000);
        double temp = 1000;
        std::cout<<"Temp"<<std::atoi(argv[2])<<std::endl;
        if (std::atoi(argv[2]) == 1){
            StartIO<TempFirst> IO(solution, mutation, temp);
            AbstractSolution* bestSolution = IO.mainAlgorithm();
            std::cout << "!!!!!!!!!!! 1" << std::endl;
            std::cout << "!!!!!!!!!!! " << std::endl;
            std::cout << "!!!!!!!!!!! " << std::endl;
            std::cout << "!!!!!!!!!!! " << std::endl;
            std::cout << "!!!!!!!!!!! " << std::endl;
            std::cout << "BEST SOLUTION: " << std::endl;
            bestSolution->print();
            std::cout<<"INIT_SOLUTION"<<solution->res<<std::endl;
            std::cout<<"BEST_SOLUTION"<<bestSolution->res<<std::endl;
        }else if(std::atoi(argv[2]) == 2){
            StartIO<TempSecond> IO(solution, mutation, temp);
            AbstractSolution* bestSolution = IO.mainAlgorithm();
            std::cout << "!!!!!!!!!!! 2" << std::endl;
            std::cout << "!!!!!!!!!!! " << std::endl;
            std::cout << "!!!!!!!!!!! " << std::endl;
            std::cout << "!!!!!!!!!!! " << std::endl;
            std::cout << "!!!!!!!!!!! " << std::endl;
            std::cout << "BEST SOLUTION: " << std::endl;
            bestSolution->print();
            std::cout<<"INIT_SOLUTION"<<solution->res<<std::endl;
            std::cout<<"BEST_SOLUTION"<<bestSolution->res<<std::endl;
        } else{
            StartIO<TempThird> IO(solution, mutation, temp);
            AbstractSolution* bestSolution = IO.mainAlgorithm();
            std::cout << "!!!!!!!!!!! 3 " << std::endl;
            std::cout << "!!!!!!!!!!! " << std::endl;
            std::cout << "!!!!!!!!!!! " << std::endl;
            std::cout << "!!!!!!!!!!! " << std::endl;
            std::cout << "!!!!!!!!!!! " << std::endl;
            std::cout << "BEST SOLUTION: " << std::endl;
            bestSolution->print();
            std::cout<<"INIT_SOLUTION"<<solution->res<<std::endl;
            std::cout<<"BEST_SOLUTION"<<bestSolution->res<<std::endl;
        }
        auto end = std::chrono::steady_clock::now();
        std::cout << "TIME WORK: "<< std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << std::endl;

    } else {
        std::cout << "Start Parallel"<< std::endl;
        std::cout << "TASK VECTOR: " << std::endl;
        std::cout<< "Tem_" << std::atoi(argv[2])<<std::endl;
        for (int i = 0; i < tasks->size(); i++)
        {
            std::cout << "ID = " << i << " PROCESSING TIME = " << (*tasks)[i] << " \n";
        }

        std::cout << std::endl;
        //AbstractSolution* ans= nullptr; 
        AbstractSolution* solution = new Solution(tasks, num_proc);
        solution->print();
        AbstractMutation* mutation = new Mutation;
        //Temp *temp = new TempFirst(1000);
        //double temp = 1000;
        auto begin = std::chrono::steady_clock::now();
        std::vector<std::thread> threadPool;
        
        const size_t MAX_ITERATION_WITHOUT_UPDATE = 10;
        size_t iterationWithoutUpdate = 0;
        int BestMetric = solution->res;
    
        while (iterationWithoutUpdate <= MAX_ITERATION_WITHOUT_UPDATE) {
            std::vector<AbstractSolution*> Buffer(numThreads);
            
            for (int i = 0; i < numThreads; i++) {
                    threadPool.emplace_back(init, solution, mutation, std::atoi(argv[2]), &(Buffer[i]), i);
            }

            for (auto& i : threadPool) {
                i.join();
            }

            delete solution;
            AbstractSolution* ans = Buffer[0]->copy_solution();

            for (int i = 0; i < numThreads; i++) {
                if (solution->res > Buffer[i]->res) {
                    iterationWithoutUpdate = 0;
                    delete ans;
                    ans = Buffer[i]->copy_solution();
                    BestMetric = ans->res;
                }
                delete Buffer[i];
            }
            solution = ans;
            Buffer.clear();
            threadPool.clear();
            iterationWithoutUpdate++;
        }
        std::cout << "BEST SOLUTION: " << std::endl;
        solution->print();
        std::cout<< "Tem_" << std::atoi(argv[2])<<std::endl;
        std::cout<<"BEST_SOLUTION :"<<solution->res<<std::endl;
        auto end = std::chrono::steady_clock::now();
        std::cout << "TIME WORK: "<< std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << std::endl;
        //printLog(ans, std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count(), PRINT_ANSWER);
        delete solution;
    }
    //solution->print();
    /*std::cout << "!!!!!!!!!!! " << std::endl;
    AbstractSolution* solution2 = solution->copy_solution();
    solution2->print();
    delete solution2;
    std::cout << "!!!!!!!!!!! " << std::endl;
    solution->print();
     std::cout << "!!!!!!!!!!! " << std::endl;
      std::cout << "!!!!!!!!!!! " << std::endl;
     std::cout << "MUTATION: " << std::endl;*/
    //AbstractMutation* mutation1 = new Mutation;

   // AbstractSolution* solution3 = mutation1->modifySolution(solution);
    //solution3->print();
    std::cout << "PROGRAM END" << std::endl;
    return 0;
}