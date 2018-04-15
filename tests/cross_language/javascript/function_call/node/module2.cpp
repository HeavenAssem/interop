//
// Created by islam on 13.05.17.
//

#include <mosaic.h>

#include <gtest/gtest.h>

using namespace std;

namespace mosaic {
    void MOSAIC_MODULE_REGISTER() {

    }

    module_context * ctx;

    void MOSAIC_MODULE_INITIALIZE(module_context & context) {
        printf("module 2 initialization\n");

        ctx = &context;
    }
}

class mosaic_test: public ::testing::Test {};


const auto other_module = "js.module";

TEST_F(mosaic_test, call) {
    EXPECT_THROW(mosaic::ctx->get("not exists"), mosaic::module_lookup_error);
    auto & module = mosaic::ctx->get(other_module);

    EXPECT_EQ(444, module.function("test1")->call<int32_t>());
    EXPECT_EQ("something", module.function("test2")->call<string>());
    EXPECT_EQ(150, module.function("add")->call<int32_t>(100, 50));
}
