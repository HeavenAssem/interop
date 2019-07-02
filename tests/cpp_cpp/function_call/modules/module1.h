#pragma once

#include <interop.h>

class test {
    int b;

  public:
    explicit test(int a);
    explicit test(double a);

    void member1();
    double member2(int arg);
    double member3(float arg1, double arg2);
    int get();
    ~test();
};
