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
public:
    void member_fn() {
        return;
    }
};

namespace mosaic {
    void _mosaic_module_register() {
        REGISTER_FUNCTION(hello_world)
        REGISTER_FUNCTION(add)
        REGISTER_FUNCTION(say_hello)
        REGISTER_MEMBER("member", &test::member_fn)
    }
}
