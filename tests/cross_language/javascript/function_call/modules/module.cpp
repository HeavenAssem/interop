//
// Created by islam on 13.05.17.
//

#include <interop.h>

#include <gtest/gtest.h>

using namespace std;

namespace interop {
void INTEROP_MODULE_REGISTER() {}

module_context_t * ctx;

void INTEROP_MODULE_INITIALIZE(module_context_t & context)
{
    printf("module 2 initialization\n");

    ctx = &context;
}
} // namespace interop

class interop_test: public ::testing::Test {};

const auto other_module = "js.module";

TEST_F(interop_test, call)
{
    EXPECT_THROW(interop::ctx->get("not exists"), interop::module_lookup_error);
    auto & module = interop::ctx->get(other_module);

    EXPECT_EQ(444, module.function("test1")->call<int32_t>());
    EXPECT_EQ("something", module.function("test2")->call<string>());
    EXPECT_EQ(150, module.function("add")->call<int32_t>(100, 50));
}
