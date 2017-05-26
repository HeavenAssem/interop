//
// Created by islam on 13.05.17.
//

#include <mosaic.h>


namespace mosaic {
    void _mosaic_module_register() {

    }

    void _mosaic_module_initialize(module_context & context) {
        context.get("module1").function("hello_world").call(15);
        printf("module 2 initialization\n");
    }
}
