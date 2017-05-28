//
// Created by islam on 13.05.17.
//

#include <mosaic.h>


namespace mosaic {
    void _mosaic_module_register() {

    }

    void _mosaic_module_initialize(module_context & context) {
        printf("module 2 initialization\n");
        try {
            context.get("module1").function("hello_world").call(15, 45);
            context.get("module1").function("hello_world").call(8, 2.5);
            printf("add from module1 returned: %i \n", context.get("module1").function("add").call<int>(10, 14));
            printf("%s\n", context.get("module1").function("say_hello").call<const char *>());
        } catch (error & e) {
            printf("%s\n", e.what());
        }
    }
}
