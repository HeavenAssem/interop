//
// Created by islam on 13.05.17.
//

#include <interop.h>

#include <gtest/gtest.h>

using namespace std;

namespace interop {
    void MOSAIC_MODULE_REGISTER() {

    }

    module_context * ctx;

    void MOSAIC_MODULE_INITIALIZE(module_context & context) {
        printf("module 2 initialization\n");

        ctx = &context;
    }
}

class mosaic_test: public ::testing::Test {};

typedef void (*hello_world_t)(int n, float k, double a);

const auto other_module = "module1";

TEST_F(mosaic_test, call) {
    EXPECT_THROW(interop::ctx->get("not exists"), interop::module_lookup_error);

    interop::ctx->get(other_module).function("hello_world")->call(15, 4.5f, 7.0);

    interop::ctx->get(other_module).function("hello_world")->call(8, 2.5f, 100.8);

    EXPECT_EQ(10, interop::ctx->get(other_module).create("test", 10)->function("get")->call<int>());

    EXPECT_EQ(18, interop::ctx->get(other_module).create("test", 18.78)->function("get")->call<int>());

    EXPECT_EQ(24, interop::ctx->get(other_module).function("add")->call<int>(10, 14));
    EXPECT_STREQ("say hello yourself!", interop::ctx->get(other_module).function("say_hello")->call<const char *>());
}

TEST_F(mosaic_test, object_create_fail) {
    auto & module = interop::ctx->get(other_module);

    EXPECT_THROW(module.create("not exists"), interop::type_lookup_error);
    EXPECT_THROW(module.create("test"), interop::function_lookup_error);
    EXPECT_THROW(module.create("test", 10, 11), interop::function_lookup_error);
}
