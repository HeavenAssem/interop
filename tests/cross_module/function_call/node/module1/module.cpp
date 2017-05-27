//
// Created by islam on 13.05.17.
//

#include <mosaic.h>
#include <iostream>


void hello_world(int n, float k) {
    std::cout << "hello_world invoked! n is: " << n << "; k is:" << k << std::endl;
}

namespace mosaic {
    void _mosaic_module_register() {
        REGISTER_FUNCTION(hello_world)
    }
}
