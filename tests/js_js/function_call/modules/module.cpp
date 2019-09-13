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

module_context_t * ctx = nullptr;

void interop_link(module_context_t & context)
{
    printf("module 2 initialization\n");

    ctx = &context;
}
} // namespace interop

class interop_test: public ::testing::Test {};

const auto other_module = "js.module1";

TEST_F(interop_test, call)
{
    auto & module = interop::ctx->get(other_module);

    EXPECT_EQ(444,
              module.get<interop::function_view_t>("get_integer_from_module_2")->call<int32_t>());
}
