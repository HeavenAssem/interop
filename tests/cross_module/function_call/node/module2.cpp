//
// Created by islam on 13.05.17.
//

#include <mosaic.h>


namespace mosaic {
    void _mosaic_module_register() {

    }

    void _mosaic_module_initialize(module_context & context) {
        try {
            context.get("module1").function("hello_world").call(15, 45);
            context.get("module1").function("hello_world").call(8, 2.5);
        } catch (error & e) {
            printf("%s", e.what());
            printf("\n");
        }
        printf("module 2 initialization\n");
    }
}
