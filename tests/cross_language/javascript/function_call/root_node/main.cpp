//
// Created by islam on 13.05.17.
//

#include <platform_factory.h>
#include <platform.h>
#include <logger.h>
#include <gtest/gtest.h>

int main(int argc, char **argv) {
    auto js = mosaic::instantiate_platform("js");

    js->initialize("/usr/lib/v8/");
    js->run();
    js->dispose();

    return 0;
//    mosaic::node application("../node");
//    application.link();
//
//    ::testing::InitGoogleTest(&argc, argv);
//
//    auto exit_code = RUN_ALL_TESTS();
//
//    application.unload();
//
//    return exit_code;
}
