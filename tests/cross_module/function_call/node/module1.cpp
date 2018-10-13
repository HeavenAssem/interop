//
// Created by islam on 13.05.17.
//

#include <interop.h>
#include <iostream>


void hello_world(int n, float k, double a) {
    std::cout << "hello_world invoked! n is: " << n << "; k is: " << k << " a is: " << a << std::endl;
}

int add(int a, int b) {
    return a + b;
}

const char * say_hello() {
    return "say hello yourself!";
}

class test {
    int b;
public:
    explicit test(int a): b(a) {
        std::cout << "test object " << this << ": constructor int called" << std::endl;
    }

    explicit test(double a): b(static_cast<int>(a)) {
        std::cout << "test object " << this << ": constructor double called" << std::endl;
    }

    void member1() {
    }

    double member2(int arg) {
        return 1.5 * arg;
    }

    double member3(float arg1, double arg2) {
        return arg2 + arg1;
    }

    int get() {
        std::cout << "get invoked! b is: " << b << "; object: " << this << std::endl;
        return b;
    }

    ~test() {
        std::cout << "test object " << this << ": destructor called" << std::endl;
    }
};

namespace interop {
    void INTEROP_MODULE_REGISTER() {
        register_function("hello_world", hello_world);
        register_function("add", add);
        register_function("say_hello", say_hello);
        register_function("non_capturing_lambda", [](int a){
            return 8080 + a;
        });

        const int value = 552;
        register_function("capturing_lambda", [value](int a){
            return value + a;
        });

        register_class<test, constructor<int>, constructor<double>>("test")
            .method<&test::member1>("member1")
            .method<&test::member2>("member2")
            .method<&test::member3>("member3")
            .method<&test::get>("get");
    }
}
