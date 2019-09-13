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

  public:
    int b;
    explicit test(int a)
      : b(a)
    {}

    int get() { return b; }
    ~test() {}
};

namespace interop {

void interop_register()
{
    register_class<test, constructor<int>>("test").member<&test::get>("get").member<&test::b>("b");
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

    EXPECT_TRUE(module.get<interop::function_view_t>("test_properties")->call<bool>());
}
