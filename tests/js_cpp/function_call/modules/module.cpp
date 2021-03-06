//
// Created by islam on 13.05.17.
//

#include <interop.h>

#include <gtest/gtest.h>

#include <iostream>

using namespace std;

namespace interop {
int get10() { return 10; }

int cpp_add(int a, int b) { return a + b; }

void interop_register()
{
    register_function("get10", get10);
    register_function("add", cpp_add);
}

module_context_t * ctx;

void interop_link(module_context_t & context)
{
    printf("module 2 initialization\n");

    ctx = &context;
}
} // namespace interop

class interop_test: public ::testing::Test {};

const auto other_module = "js.module";

TEST_F(interop_test, call)
{
    EXPECT_THROW(interop::ctx->get("not exists"), interop::module_lookup_error_t);
    auto & module = interop::ctx->get(other_module);

    EXPECT_EQ(444, module.get<interop::function_view_t>("test1")->call<int32_t>());
    EXPECT_EQ("something", module.get<interop::function_view_t>("test2")->call<string>());
    EXPECT_EQ(150, module.get<interop::function_view_t>("add")->call<int32_t>(100, 50));
}

TEST_F(interop_test, native_module_visibility)
{
    auto & module = interop::ctx->get(other_module);

    EXPECT_TRUE(module.get<interop::function_view_t>("check_modules")->call<bool>());
}

TEST_F(interop_test, native_module_call)
{
    auto & module = interop::ctx->get(other_module);

    EXPECT_EQ(10, module.get<interop::function_view_t>("cpp_get10")->call<int>());
    EXPECT_EQ(15, module.get<interop::function_view_t>("cpp_add")->call<int>(10, 5));
}
