#include "lib.h"

int main() {

    FuncFactory r;
    try {
        auto a2 = r.CreateFunction("ident", {0, 0});
        auto a3 = r.CreateFunction("const", {1.5});
        auto a4 = r.CreateFunction("power", {0});
        auto a6 = r.CreateFunction("exp", {3});

        std::cout << (*a2).ToString()<< " = " << (*a4)(2.)<<std::endl;
        std::cout << (*a3).ToString()<<std::endl;
        std::cout << (*a4).ToString()<<std::endl;
        std::cout << (*a6).ToString()<<std::endl;

        std::cout << (*a3 * *a2).ToString()<<std::endl;
        std::cout << (*a2 + *a2 + *a3 + *a4).ToString()<<std::endl;
        std::cout << ((*a3 + *a2 - *a6) / *a3).ToString()<<std::endl;
        auto a7 = ((*a2 + *a3) * *a6) / *a3;
        std::cout << a7(0) << std::endl;
        std::cout << a7.GetDerive(0)<<std::endl;

    }
    catch (std::logic_error& err) {
        std::cerr << "logic_error" << std::endl;
    }
    return 0;
}

