#ifndef LIBRARY_H
#define LIBRARY_H


#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <vector>
#include <type_traits>
#include <initializer_list>
#include <stdexcept>
#include <map>


template<typename T>
std::string toStr(T value) {
    std::ostringstream s;
    s.precision(std::numeric_limits<T>::digits10);
    s << value;
    return s.str();
}


class IFunction {
public:
	virtual std::string ToString() const = 0;
    virtual double operator()(const double& x) const = 0;
    virtual double GetDerive(const double& x) const = 0;
    virtual IFunction* clone() const = 0;
    virtual ~IFunction() = default;

protected:
    std::string str;
};


enum Operation {
    ADD,
    SUB,
    MUL,
    DIV
};

class OperationsFun : public IFunction {
    std::unique_ptr<IFunction> Operand1;
    std::unique_ptr<IFunction> Operand2;
    Operation Op;
public:
    OperationsFun (const IFunction& O1, const IFunction& O2, Operation O) : Operand1(O1.clone()), Operand2(O2.clone()), Op(O) {}

    std::string ToString () const override {
        std::string Res = Operand1->ToString();
        switch (Op) {
            case ADD:
                Res += std::string(" + ");
                break;
            case SUB:
                Res += std::string(" - ");
                break;
            case MUL:
                Res += std::string(" * ");
                break;
            case DIV:
                Res += std::string(" / ");
                break;
        }
        Res += Operand2->ToString();
        return Res;
    }

    double GetDerive(const double& Point) const override {
        double Res = 0;
        switch (Op) {
            case ADD:
                Res += Operand1->GetDerive(Point) + Operand2->GetDerive(Point);
                break;
            case SUB:
                Res += Operand1->GetDerive(Point) + Operand2->GetDerive(Point);
                break;
            case MUL:
                Res += Operand1->GetDerive(Point) * Operand2->operator()(Point)
                       + Operand2->GetDerive(Point) * Operand1->operator()(Point);
                break;
            case DIV:
                Res += (Operand1->GetDerive(Point) * Operand2->operator()(Point)
                        - Operand2->GetDerive(Point) * Operand1->operator()(Point))
                       / pow(Operand2->operator()(Point), 2);
                break;
        }
        return Res;
    }
    double operator() (const double& Point) const override {
        double Res = 0;
        switch (Op) {
            case ADD:
                Res += Operand1->operator()(Point) + Operand2->operator()(Point);
                break;
            case SUB:
                Res += Operand1->operator()(Point) - Operand2->operator()(Point);
                break;
            case MUL:
                Res += Operand1->operator()(Point) * Operand2->operator()(Point);
                break;
            case DIV:
                Res += Operand1->operator()(Point) / Operand2->operator()(Point);
                break;
        }
        return Res;
    }

    OperationsFun(const OperationsFun& a) {
        Operand1 = std::unique_ptr<IFunction>(a.Operand1->clone());
        Operand2 = std::unique_ptr<IFunction>(a.Operand2->clone());
        Op = a.Op;
    }

    IFunction* clone() const override {
        return new OperationsFun(*this);
    }

    // virtual ~OperationsFun() = default;
};


class Ident : public IFunction{
public:
	Ident(const std::vector<double>& y){
        str = "x";
	};
	std::string ToString () const override {
        return str;
    }
    double operator() (const double& x) const override {
        return x;
    }
    double GetDerive(const double& y) const override {
        return 1;
    }
	IFunction* clone() const override{
        return new Ident(*this);
    }
	
};


class Const : public IFunction{
	double val;
public:
	Const(const std::vector<double>& y): val(y[0]){
        str = toStr(val);
	};

	std::string ToString () const override {
        return str;
    }
    double operator() (const double& y) const override {
        return val;
    }
    double GetDerive(const double& y) const override {
        return 0;
    }
	IFunction* clone() const override{
        return new Const(*this);
    }	
};

// x^a
class Power : public IFunction{
    double val;
public:
    Power(const std::vector<double>& y): val(y[0]){
        str = "x^" + toStr(val);
    };

    std::string ToString () const override {
        return str;
    }
    double operator() (const double& x) const override {
        return pow(x, val);
    }
    double GetDerive(const double& y) const override {
        return val * pow(y, val - 1);
    }
    IFunction* clone() const override{
        return new Power(*this);
    }   
};

// e^x
class Exp : public IFunction{
    double val;
public:
    Exp(const std::vector<double>& y): val(y[0]){
        str = "e^" + toStr(val) + "x";
    };

    std::string ToString () const override {
        return str;
    }
    double operator() (const double& y) const override {
        return std::exp(val * y);
     }
    double GetDerive(const double& y) const override {
        return val * std::exp(val * y);
    }
    IFunction* clone() const override{
        return new Exp(*this);
    }   
};

class Polynomial : public IFunction{
private:
    std::vector<double> m_vec, d_vec;
public:
    Polynomial(const std::vector<double>& mVec) : m_vec(mVec) {
        if(!mVec.empty()){
            int c = 0;
            for (auto i : mVec) {
                d_vec.push_back(c * i);
                str += toStr(i) + std::string("x^") + toStr(c) + std::string("+");
                c++;
            }
            str.pop_back();
        }else{
            throw std::logic_error("not enough elements to construct polynomial");
        }
    };

    std::string ToString () const override {
        return str;
    }

    double operator()(const double& x) const override {
        double xx = 1;
        double res = 0;
        for (auto i : m_vec){
            res += xx * i;
            xx *= x;
        }
        return res;
    }

    double GetDerive(const double& x) const override {
        double res = 0;
        for (int i = 1; i < m_vec.size(); i++){
            res += m_vec[i] * i * pow(x, i - 1);
        }
        return res;
    }

    IFunction* clone() const override{
        return new Polynomial(*this);
    }
};

class FuncFactory{
public:
    
    FuncFactory(){
        RegisterAll();
    };

    class ICreator{
    public:
        virtual std::unique_ptr<IFunction> create(std::vector<double>) = 0;
        virtual ~ICreator(){};
    };

    template<typename T>
    class TCreator : public ICreator{
    public:
        std::unique_ptr<IFunction> create(std::vector<double> vec)override {
            return std::make_unique<T>(vec);
        }
    };
    using Registered = std::map<std::string, std::shared_ptr<ICreator>>;

    Registered rmap;

    template<typename T>
    void Register(std::string s){
        rmap[s] = std::make_shared<TCreator<T>>();
    }

    void RegisterAll(){
        Register<Const>("const");
        Register<Polynomial>("polynomial");
        Register<Exp>("exp");
        Register<Power>("power");
        Register<Ident>("ident");
    }

    std::unique_ptr<IFunction> CreateFunction(std::string s, std::vector<double> vec){
        auto creator = rmap.find(s);
        if (creator == rmap.end()) {
            return nullptr;
        }
        return creator->second->create(vec);
    }
};


OperationsFun operator+(const IFunction& a, const IFunction& b) {
    return OperationsFun(a, b, ADD);
}


OperationsFun operator-(const IFunction& a, const IFunction& b) {
    return OperationsFun(a, b, SUB);
}


OperationsFun operator*(const IFunction& a, const IFunction& b) {
    return OperationsFun(a, b, MUL);
}


OperationsFun operator/(const IFunction& a, const IFunction& b) {
    return OperationsFun(a, b, DIV);
}


template<class T1, class T2>
std::enable_if_t<
!std::is_base_of<IFunction, std::decay_t<T1>>::value xor
!std::is_base_of<IFunction, std::decay_t<T2>>::value, OperationsFun>
operator+(T1&& a, T2&& b) {
    throw std::logic_error("error");
}

template<class T1, class T2>
std::enable_if_t<
!std::is_base_of<IFunction, std::decay_t<T1>>::value xor
!std::is_base_of<IFunction, std::decay_t<T2>>::value, OperationsFun> 
operator-(T1&& a, T2&& b) {
    throw std::logic_error("error");
}

template<class T1, class T2>
std::enable_if_t<
!std::is_base_of<IFunction, std::decay_t<T1>>::value xor
!std::is_base_of<IFunction, std::decay_t<T2>>::value, OperationsFun> 
operator*(T1&& a, T2&& b) {
    throw std::logic_error("error");
}

template<class T1, class T2>
std::enable_if_t<
!std::is_base_of<IFunction, std::decay_t<T1>>::value xor
!std::is_base_of<IFunction, std::decay_t<T2>>::value, OperationsFun> 
operator/(T1&& a, T2&& b) {
    throw std::logic_error("error");
}

double getRoot(const IFunction& f, int it=1000, double x0 = -2., 
               double lambda = 0.01, double eps = 0.00001) {
    double f1 = f(x0);
    for (int i = 1; i <= it and std::abs(f1) > eps; i++) {
        if (f1 > 0){
            x0 = x0 + lambda * (-f.GetDerive(x0));
        }else{
            x0 = x0 + lambda * f.GetDerive(x0);
        }
       
        f1 = f(x0);
    }
    return x0;
}

#endif