#include "gtest/gtest.h"
#include "lib.h"

TEST(Function, Init) {
    FuncFactory f;
    auto exp = f.CreateFunction("exp", {2.7});
    auto ident = f.CreateFunction("ident", {});
    auto power1 = f.CreateFunction("power", {0});
    auto power2 = f.CreateFunction("power", {1, 1});
    auto c = f.CreateFunction("const", {-14.3});
    auto pol = f.CreateFunction("polynomial", {0, 1, 2});

    ASSERT_STREQ(exp->ToString().c_str(), "e^2.7x");
    ASSERT_STREQ(ident->ToString().c_str(), "x"); 
    ASSERT_STREQ(power1->ToString().c_str(), "x^0");
    ASSERT_STREQ(power2->ToString().c_str(), "x^1");
    ASSERT_STREQ(c->ToString().c_str(), "-14.3");
    ASSERT_STREQ(pol->ToString().c_str(), "0x^0+1x^1+2x^2");
};

TEST(Function, Operation) {
    FuncFactory f;
    auto exp = f.CreateFunction("exp", {2.7});
    auto ident = f.CreateFunction("ident", {});
    auto power1 = f.CreateFunction("power", {0});
    auto power2 = f.CreateFunction("power", {1, 1});
    auto c = f.CreateFunction("const", {-14.3});
    auto pol = f.CreateFunction("polynomial", {0, 1, 2});

    ASSERT_NEAR((*exp)(0), 1, 0.00001);
    ASSERT_NEAR((*ident)(2), 2, 0.00001); 
    ASSERT_NEAR((*power1)(7), 1, 0.00001);
    ASSERT_NEAR((*power2)(7), 7, 0.00001);
    ASSERT_NEAR((*c)(0), -14.3, 0.00001);
    ASSERT_NEAR((*pol)(1), 3, 0.00001);

    ASSERT_NEAR(ident->GetDerive(2), 1, 0.00001); 
    ASSERT_NEAR(c->GetDerive(0), 0, 0.00001);
    ASSERT_NEAR(pol->GetDerive(1), 5, 0.00001);

    auto complex1 = *exp + *pol;
    auto complex2 = *ident - *pol;
    auto complex3 = *c * *pol;
    auto complex4 = *power1 / *power2;
    auto complex5 = (complex1 + complex2);
    
    ASSERT_THROW(complex5 + std::string(""), std::logic_error);

    ASSERT_NEAR(complex1(1), 17.8797, 0.0001);
    ASSERT_NEAR(complex2(1), -2, 0.00001);
    ASSERT_NEAR(complex3(1), -42.9, 0.00001);
    ASSERT_NEAR(complex4(1), 1, 0.00001);
    ASSERT_NEAR(complex5(1), 15.8797, 0.0001);

    ASSERT_NEAR(complex1.GetDerive(0), 3.7, 0.00001);
    ASSERT_NEAR(complex2.GetDerive(1), 6, 0.00001);
    ASSERT_NEAR(complex3.GetDerive(0), -14.3, 0.00001);
    ASSERT_NEAR(complex4.GetDerive(1), -1, 0.0001);
};

TEST(Function, FindRoot) {
    FuncFactory f;
    auto pol1 = f.CreateFunction("polynomial", {1, 1});
    auto pol2 = f.CreateFunction("polynomial", {1, 2, 1});

    ASSERT_NEAR(getRoot(*pol1, 1000, -10), -1, 0.01);
    ASSERT_NEAR(getRoot(*pol2, 1000, 10), -1, 0.01);
};
