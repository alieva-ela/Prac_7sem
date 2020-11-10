#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <iostream>

class Temp {
public:
    //virtual Temp(double init_temp) = 0;
    virtual double getDecreaseTemp(int iteration) = 0 ;
};

// T = T0 / ln(1 + i)
class TempFirst : public Temp {
    double init_temp;
public:
    TempFirst(double init_temp = 0.0) : init_temp(init_temp) {
    }
    
    virtual double getDecreaseTemp(int iteration) override {
        return init_temp / std::log(1 + iteration);
    }
};

// T = T0 / (1 + i)
class TempSecond : public Temp {
    double init_temp;
public:
    TempSecond(double init_temp = 0.0) : init_temp(init_temp) {
        std::cout<< "Tem2"<<std::endl;
    }
    
    virtual double getDecreaseTemp(int iteration) override {
        return init_temp / (1 + iteration);
    }
};

// T = T0 * ln(1 + i) / (1 + i)
class TempThird : public Temp {
    double init_temp;
public:
    TempThird(double init_temp=0.0) : init_temp(init_temp) {
    }
    
    virtual double getDecreaseTemp(int iteration) override {
        return init_temp * (std::log(1 + iteration) / (1 + iteration));
    }
};


#endif //TEMP_HPP