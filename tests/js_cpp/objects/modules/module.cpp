//
// Created by islam on 13.05.17.
//

#include <interop.h>

// for testing only
#include <platforms/javascript_v8/module.h>

#include <gtest/gtest.h>

#include <iostream>

using namespace std;

class test {
    int b;

    static uint objects_count;

  public:
    explicit test(int a)
      : b(a)
    {
        ++objects_count;
    }
    explicit test(double a)
      : b(a + 1)
    {
        ++objects_count;
    }

    int add1(int arg) const { return arg + b; }
    int add2(int arg1, int arg2) const { return arg1 + arg2 + b; }
    int mul_mod(int arg) { return b *= arg; }
    int get() { return b; }
    ~test() { --objects_count; }

    static uint get_objects_count() { return objects_count; }
};

uint test::objects_count = 0;

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

const auto js_module_name_c = "js.module";

TEST(objects, cpp_to_js)
{
    auto & module = interop::ctx->get(js_module_name_c);

    EXPECT_TRUE(module.get<interop::function_view_t>("test_objects")->call<bool>());
}

TEST(objects, lifetime)
{
    auto & module =
        dynamic_cast<interop::platform_v8::module_t &>(interop::ctx->get(js_module_name_c));

    EXPECT_TRUE(module.get<interop::function_view_t>("test_objects")->call<bool>());

    module.initiate_garbage_collection_for_testing();

    EXPECT_EQ(0, test::get_objects_count());
}

TEST(objects, js_to_cpp)
{
    auto & js_module = interop::ctx->get(js_module_name_c);

    {
        auto js_object = js_module.create_dynamic("TestObject", {1, 2, 3});

        EXPECT_EQ(1, js_object->get<interop::function_view_t>("get_a")->call<int>());
        EXPECT_EQ(2, js_object->get<interop::function_view_t>("get_b")->call<int>());
        EXPECT_EQ(3, js_object->get<interop::function_view_t>("get_c")->call<int>());
    }

    {
        auto js_object = js_module.create_dynamic("NewTypeClass", {1, "some value"});

        EXPECT_EQ(3, js_object->get<interop::function_view_t>("get_a")->call<int>());
        EXPECT_EQ("some value was passed from c++",
                  js_object->get<interop::function_view_t>("get_b")->call<std::string>());
    }
}
