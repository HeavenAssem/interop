//
// Created by islam on 13.05.17.
//

#include <mosaic.h>

#include <gtest/gtest.h>


namespace mosaic {
    void _mosaic_module_register() {

    }

    module_context * ctx;

    void _mosaic_module_initialize(module_context & context) {
        printf("module 2 initialization\n");

        ctx = &context;
    }
}

class mosaic_test: public ::testing::Test {

};


TEST_F(mosaic_test, call) {
    mosaic::ctx->get("module1").function("hello_world")->call(15, 4.5f, 7.0);

    mosaic::ctx->get("module1").function("hello_world")->call(8, 2.5f, 100.8);

    EXPECT_EQ(10, mosaic::ctx->get("module1").create("test", 10)->function("get")->call<int>());

    EXPECT_EQ(18, mosaic::ctx->get("module1").create("test", 18.78)->function("get")->call<int>());

    EXPECT_EQ(24, mosaic::ctx->get("module1").function("add")->call<int>(10, 14));
    EXPECT_STREQ("say hello yourself!", mosaic::ctx->get("module1").function("say_hello")->call<const char *>());
}

TEST_F(mosaic_test, object_create_fail) {
    auto & module = mosaic::ctx->get("module1");

    EXPECT_THROW(module.create("not exists"), mosaic::type_lookup_error);
    EXPECT_THROW(module.create("test"), mosaic::function_lookup_error);
    EXPECT_THROW(module.create("test", 10, 11), mosaic::function_lookup_error);
}