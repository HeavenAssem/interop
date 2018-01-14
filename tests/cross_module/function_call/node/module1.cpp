//
// Created by islam on 13.05.17.
//

#include <mosaic.h>
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

namespace mosaic {
    void MOSAIC_MODULE_REGISTER() {
        REGISTER_FUNCTION(hello_world)
        REGISTER_FUNCTION(add)
        REGISTER_FUNCTION(say_hello)
        //REGISTER_MEMBER("member", &test::member_fn)
        register_class<test, constructor<int>, constructor<double>>("test")
            .method<&test::member1>("member1")
            .method<&test::member2>("member2")
            .method<&test::member3>("member3")
            .method<&test::get>("get");
    }
}
