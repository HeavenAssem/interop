//
// Created by islam on 13.05.17.
//

#include <interop.h>

#include <gtest/gtest.h>

#include <iostream>

using namespace std;

class test {
    int b;

  public:
    explicit test(int a)
      : b(a)
    {}
    explicit test(double a)
      : b(a + 1)
    {}

    int add1(int arg) const { return arg + b; }
    int add2(int arg1, int arg2) const { return arg1 + arg2 + b; }
    int mul_mod(int arg) { return b *= arg; }
    int get() { return b; }
    ~test();
};

namespace interop {

void interop_register()
{
    register_class<test, constructor<int>, constructor<double>>("test")
        .member<&test::add1>("add1")
        .member<&test::add2>("add2")
        .member<&test::mul_mod>("mul_mod")
        .member<&test::get>("get");
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

TEST_F(interop_test, objects)
{
    auto & module = interop::ctx->get(other_module);

    EXPECT_TRUE(module.function("test_objects")->call<bool>());
}
