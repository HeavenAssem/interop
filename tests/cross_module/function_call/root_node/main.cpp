//
// Created by islam on 13.05.17.
//

#include <node.h>
#include <logger.h>
#include <gtest/gtest.h>

int main(int argc, char **argv) {
    mosaic::node application("../node");
    application.link();

    ::testing::InitGoogleTest(&argc, argv);

    auto exit_code = RUN_ALL_TESTS();

    application.unload();

    return exit_code;
}
